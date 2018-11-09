//
// Created by yqt on 18-11-9.
//

#ifndef ALL_DBMYSQLTASKPOOL_H
#define ALL_DBMYSQLTASKPOOL_H

#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <stack>
#include <functional>
#include <condition_variable>
#include "ImPdu.h"
#include "Singleton.h"
#include "databasemysql.h"

typedef std::function<void(CImPdu* pdu)> PDUDataCallback;

class ITaskDB
{
public:
    ITaskDB(void) {};
    virtual ~ITaskDB(void) {};
public:
    virtual void Process(std::shared_ptr<CDatabaseMysql> dbConn) = 0;
    virtual void SetDataCallBack(PDUDataCallback cb);
    virtual void SetId(uint64_t id);
protected:
    uint64_t id_;
    PDUDataCallback cb_;
};

class TaskDBThdPool;
class TaskDBThd
{
public:
    TaskDBThd(TaskDBThdPool *Thdpool)
            :threadpool_(Thdpool) {}
    ~TaskDBThd() = default;
    void Initialize(const std::string& host, const string& user, const string& pwd, const string& dbname);
    void Assign(std::shared_ptr<ITaskDB> ptask) { task_ = ptask; }

    void Run();
    void Start();
private:
    std::shared_ptr<ITaskDB> task_;
    std::thread thread_;
    TaskDBThdPool *threadpool_;
    std::shared_ptr<CDatabaseMysql> poConn_;
};

class TaskDBThdPool
    : public Singleton<TaskDBThdPool>
{
public:
    TaskDBThdPool();
    ~TaskDBThdPool();
    bool start();
    void stop();
    void run();
    void AddReq(std::shared_ptr<ITaskDB> req);
    void SetThreadidl(TaskDBThd *myThread);
    void release();
private:
    int							thread_num = 10; //线程数 每个线程与mysql建立一个连接
    std::mutex                  task_mutex_;    // 任务队列锁
    std::mutex                  thread_mutex_;  // 线程队列锁
    std::condition_variable     cond_;          // 条件阻塞
    std::thread                 thread_;        // 调度线程
    std::atomic<bool>           stop_ = false;  // 是否关闭提交
    std::queue<std::shared_ptr<ITaskDB>>  taskdeque_;
    std::stack<TaskDBThd*>     idel_threads_;   // 空闲线程队列
    typedef std::chrono::duration<int> seconds_type;
    seconds_type seconds_wait;
};

#endif //ALL_DBMYSQLTASKPOOL_H
