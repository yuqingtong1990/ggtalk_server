//
// Created by yqt on 18-11-19.
//

#ifndef ALL_CHATSERVER_H
#define ALL_CHATSERVER_H

#include <memory>
#include <vector>
#include <map>
#include <atomic>
#include <mutex>
#include <list>

#include "ClientSession.h"


class chatServer {
public:
    chatServer();
    ~chatServer();

    bool Init(std::string& strErrMsg);
    void End();

    void OnConnection(const evpp::TCPConnPtr& conn);
    bool Start(evpp::EventLoop* loop,const char* ip = "0.0.0.0", short port =8100);
    void Stop();
    void Loop();
    void OnClose(const evpp::TCPConnPtr& conn);

private:
    std::mutex mutex_session_;
    typedef std::list<std::shared_ptr<ClientSession>> ConnectionList;
	ConnectionList connections_;
    ManyOnlineType Onlinetype_;

    std::shared_ptr<evpp::TCPServer> server_;
    evpp::EventLoop* loop_ = nullptr;
    evpp::InvokeTimerPtr ptrSynscdn_ = nullptr;
};


#endif //ALL_CHATSERVER_H
