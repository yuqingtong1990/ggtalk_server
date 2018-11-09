//
// Created by yqt on 18-11-8.
//

#include "db_server.h"
using namespace evpp;

DBServer::DBServer() {
    isRun_.store(false);
}

DBServer::~DBServer() {

}

void DBServer::OnConnection(const evpp::TCPConnPtr &conn) {

    LOG_INFO << conn->AddrToString() << " is " << (conn->IsConnected() ? "UP" : "DOWN");
    if (conn->IsConnected())
    {
        LOG_INFO << "A new connection from " << conn->remote_addr();
        std::lock_guard<std::mutex> guard(mutex_conn_);
        mapIdConn_[conn->id()] = conn;
    } else {
        OnClose(conn);
        LOG_INFO << "Lost the connection from " << conn->remote_addr();
    }
}

void DBServer::OnChatMessage(const evpp::TCPConnPtr &conn, evpp::Buffer *msg) {
    //在这里接收消息服务器过来的消息
    int64_t id = conn->id();//用于回复

}

bool DBServer::Start(evpp::EventLoop *loop, const char *ip, short port) {
    bool bRet = false;
    if (loop == nullptr)
        return false;

    char ch[100] = { 0 };
    snprintf(ch, 100, "%s:%d", ip, port);
    tcpserver_.reset(new TCPServer(loop, ch, "chatserver", 10));
    bRet = tcpserver_->Init();
    if (!bRet)
    {
        LOG_ERROR << "chatserver bind fail";
        return false;
    }

    tcpserver_->SetConnectionCallback(std::bind(&DBServer::OnConnection, this, std::placeholders::_1));
	tcpserver_->SetMessageCallback(std::bind(&DBServer::OnChatMessage, this, std::placeholders::_1, std::placeholders::_2));
    bRet = tcpserver_->Start();
    if (!bRet)
    {
        LOG_ERROR << "chatserver bind fail";
        return false;
    }
     isRun_ = true;
    return true;
}

void DBServer::Loop() {
    loop_->Run();
}

void DBServer::OnClose(const evpp::TCPConnPtr &conn) {
    std::lock_guard<std::mutex> guard(mutex_conn_);

    if (mapIdConn_.size())
        return;

    auto iter = mapIdConn_.find(conn->id());
    if (iter != mapIdConn_.end()){
        mapIdConn_.erase(iter++);
    }
}
