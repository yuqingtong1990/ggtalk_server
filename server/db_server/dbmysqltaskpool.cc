//
// Created by yqt on 18-11-9.
//
#include "DbSrvConfigMgr.h"
#include "dbmysqltaskpool.h"

void ITaskDB::SetId(uint64_t id) {
    id_ = id;
}

void ITaskDB::SetDataCallBack(PDUDataCallback cb) {
    cb_ = cb;
}

void TaskDBThd::Initialize(const std::string &host, const string &user, const string &pwd, const string &dbname) {
    poConn_.reset(new CDatabaseMysql());
    poConn_->Initialize(host, user, pwd, dbname);
}

void TaskDBThd::Run() {
    task_->Process(poConn_);
    threadpool_->SetThreadidl(this);
}

void TaskDBThd::Start() {
    thread_ = std::thread(&TaskDBThd::Run, this);
    thread_.detach();
}
TaskDBThdPool::TaskDBThdPool()
:seconds_wait(1)
{
    stop_.store(false);
    int thread_num = std::thread::hardware_concurrency() * 2;
		thread_num = (thread_num == 0 ? 2 : thread_num);
    //创建空闲线程
    for (int i = 0; i != thread_num; ++i)
    {
        TaskDBThd* pthread = new TaskDBThd(this);
        pthread->Initialize(mysqlDbConfig::getSingleton().host(), mysqlDbConfig::getSingleton().user(), mysqlDbConfig::getSingleton().password(), mysqlDbConfig::getSingleton().dbname());
        idel_threads_.push(pthread);
    }
}

TaskDBThdPool::~TaskDBThdPool() {

}

bool TaskDBThdPool::start() {
    thread_ = std::thread(&TaskDBThdPool::run, this);
    thread_.detach();
    return true;
}

void TaskDBThdPool::stop() {
    stop_.store(true);
    cond_.notify_all();
    release();
}

void TaskDBThdPool::run() {
    while (true)
    {
        if (stop_.load())
            break;

        {
            std::unique_lock<std::mutex> ulk(task_mutex_);
            this->cond_.wait_for(ulk, seconds_wait, [this] { return/* stop_.load() ||*/ !this->taskdeque_.empty(); });

            if (stop_.load())
                break;

            if (this->taskdeque_.empty())
                continue;

            {
                std::unique_lock<std::mutex> ulk(thread_mutex_);
                if (idel_threads_.empty())
                {
                    continue;
                }
                //获取空闲队列
                TaskDBThd* p = idel_threads_.top();
                idel_threads_.pop();

                p->Assign(taskdeque_.front());
                taskdeque_.pop();
                p->Start();
            }
        }
    }
}

void TaskDBThdPool::AddReq(std::shared_ptr<ITaskDB> req) {
    std::unique_lock<std::mutex> ulk(task_mutex_);
    taskdeque_.push(req);
    cond_.notify_one();
}

void TaskDBThdPool::SetThreadidl(TaskDBThd *myThread) {
    if (stop_)
    {
        delete myThread;
    }
    else
    {
        std::unique_lock<std::mutex> ulk(thread_mutex_);
        idel_threads_.push(myThread);
        cond_.notify_all();
    }
}

void TaskDBThdPool::release() {
    std::unique_lock<std::mutex> ulk(thread_mutex_);
    TaskDBThd* pTaskthread = NULL;
    do
    {
        if (idel_threads_.empty())
            break;

        pTaskthread = idel_threads_.top();
        if (pTaskthread) {
            delete pTaskthread;
            idel_threads_.pop();
        }
        else {
            idel_threads_.pop();
        }
    } while (pTaskthread);
}


