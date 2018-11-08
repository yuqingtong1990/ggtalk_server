//
// Created by yqt on 18-11-6.
//

#include "cdn_server.h"
#include "json/json.h"

using namespace evpp;

CDNServer::CDNServer() {
    isRun_.store(false);
}

void CDNServer::OnConnection(const evpp::TCPConnPtr &conn) {

    if (conn->IsConnected()) {
            LOG_INFO << "A new connection from " << conn->remote_addr();
        } else {
            LOG_INFO << "Lost the connection from " << conn->remote_addr();
        }
}

bool CDNServer::Start(evpp::EventLoop *loop, const char *ip, short port) {

    bool bRet = false;
    if (loop == nullptr)
        return bRet;

    loop_ = loop;

    char ch[100] = { 0 };
    snprintf(ch, 100, "%s:%d", ip, port);

    tcpserver_.reset(new TCPServer(loop,ch,"cdn",10));
    bRet = tcpserver_->Init();
    if (!bRet){
        LOG_ERROR << "cdn bind port fail";
        return false;
    }
    tcpserver_->SetConnectionCallback(std::bind(&CDNServer::OnConnection, this, std::placeholders::_1));
    tcpserver_->SetMessageCallback(std::bind(&CDNServer::OnChatMessage,this,std::placeholders::_1,std::placeholders::_2));
    bRet = tcpserver_->Start();
    if (!bRet)
    {
        LOG_ERROR << "cdn start tcp_server fail";
        return false;
    }

    std::vector<int> ports = { 10000,10001 };
    httpServer_.reset(new http::Server(4));
    httpServer_->RegisterDefaultHandler([](evpp::EventLoop* loop,
                              const evpp::http::ContextPtr& ctx,
                              const evpp::http::HTTPSendResponseCallback& cb){
                                Json::Value ret;
                                cb(ret.toStyledString()); }
    );
    httpServer_->Init(ports);
    httpServer_->Start();
    //loop->RunAfter(Duration(1.0), std::bind(&CDNServer::OnSyncCdn, this));
    isRun_.store(true);
    return true;
}

void CDNServer::Loop() {
    loop_->Run();
}

void CDNServer::OnChatMessage(const evpp::TCPConnPtr &conn, evpp::Buffer *msg) {
    LOG_DEBUG << msg->ToString();

    Json::Value ret;
    ret["success"] = true;
    ret["unix_time"] = evpp::Timestamp::Now().Unix();
    conn->Send(ret.toStyledString());
}

