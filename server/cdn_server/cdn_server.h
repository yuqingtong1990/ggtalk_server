//
// Created by yqt on 18-11-6.
//

#ifndef ALL_CDN_SERVER_H
#define ALL_CDN_SERVER_H

#include <memory>
#include <vector>
#include <map>
#include <atomic>
#include <mutex>

#include "evpp/buffer.h"
#include "evpp/event_loop.h"
#include "evpp/tcp_server.h"
#include "evpp/tcp_conn.h"
#include "evpp/tcp_callbacks.h"
#include "evpp/http/http_server.h"
#include "evpp/http/context.h"

class CDNServer {
public:
    CDNServer();
    ~CDNServer() = default;

    void OnConnection(const evpp::TCPConnPtr& conn);
    void OnChatMessage(const evpp::TCPConnPtr& conn,evpp::Buffer* msg);
    bool Start(evpp::EventLoop* loop,const char* ip = "0.0.0.0", short port =8100);
    void Loop();
private:
    CDNServer(const CDNServer& rhs) = delete;
    CDNServer& operator =(const CDNServer& rhs) = delete;
private:
    std::atomic_bool isRun_;
    std::shared_ptr<evpp::TCPServer> tcpserver_;
    std::shared_ptr<evpp::http::Server> httpServer_;
    evpp::EventLoop* loop_ = nullptr;
    //InvokeTimerPtr ptrSynscdn_ = nullptr;
};


#endif
