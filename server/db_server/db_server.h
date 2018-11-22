//
// Created by yqt on 18-11-8.
//

#ifndef ALL_DB_SERVER_H
#define ALL_DB_SERVER_H

#include <memory>
#include <vector>
#include <map>
#include <atomic>
#include <mutex>
#include "base/ImPdu.h"

#include "evpp/buffer.h"
#include "evpp/event_loop.h"
#include "evpp/tcp_server.h"
#include "evpp/tcp_conn.h"
#include "evpp/tcp_callbacks.h"
#include "evpp/http/http_server.h"
#include "evpp/http/context.h"

class DBServer {
public:
    DBServer();
    ~DBServer();
    bool Init(std::string& strErrMsg);
    void End();
    void OnConnection(const evpp::TCPConnPtr& conn);
    bool Start(evpp::EventLoop* loop,const char* ip = "0.0.0.0", short port =8100);
    void Loop();
    void OnClose(const evpp::TCPConnPtr& conn);
    void OnChatMessage(const evpp::TCPConnPtr& conn,evpp::Buffer* msg);
    void CallBackPdu(uint64_t id,std::shared_ptr<CImPdu> ppdu);

private:
    bool InitRedis();
    bool InitMongoDbMgr();
    bool InitMysqlDbMgr();
private:
    std::mutex mutex_conn_;
    std::map<uint64_t,evpp::TCPConnPtr> mapIdConn_;
    std::shared_ptr<evpp::TCPServer> tcpserver_;
    evpp::EventLoop* loop_ = nullptr;
    std::atomic_bool isRun_;
};


#endif //ALL_DB_SERVER_H
