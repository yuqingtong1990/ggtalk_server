//
// Created by yqt on 18-11-9.
//

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
    int thread_num = std::thread::hardware_concurrency() * 2;
		thread_num = (thread_num == 0 ? 2 : thread_num);
    //创建空闲线程
    for (int i = 0; i != thread_num; ++i)
    {
        TaskDBThd* pthread = new TaskDBThd(this);
        pthread->Initialize(DBCfg::get().host(), DBCfg::get().user(), DBCfg::get().password(), DBCfg::get().dbname());
        idel_threads_.push(pthread);
    }
}

TaskDBThdPool::~TaskDBThdPool() {

}

bool TaskDBThdPool::start() {
    return false;
}

void TaskDBThdPool::stop() {

}

void TaskDBThdPool::run() {

}

void TaskDBThdPool::AddReq(std::shared_ptr<ITaskDB> req) {

}

void TaskDBThdPool::SetThreadidl(TaskDBThd *myThread) {

}

void TaskDBThdPool::release() {

}


