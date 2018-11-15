//
// Created by yqt on 18-11-14.
//

#ifndef ALL_PIC_SERVER_H
#define ALL_PIC_SERVER_H

#include "evpp/http/http_server.h"
#include "evpp/event_loop.h"

class PicServer {
public:
    PicServer() = default;
    ~PicServer() = default;

    bool Start(evpp::EventLoop* loop, short port =8100);
    void Loop();

    void DefaultHandler(evpp::EventLoop* loop,\
                    const evpp::http::ContextPtr& ctx,\
                    const evpp::http::HTTPSendResponseCallback& cb);

    void ProxyHandler(evpp::EventLoop* loop,\
                  const evpp::http::ContextPtr& ctx,\
                  const evpp::http::HTTPSendResponseCallback& cb);
private:
    std::shared_ptr<evpp::http::Server> httpServer_;
    evpp::EventLoop* loop_ = nullptr;
};


#endif //ALL_PIC_SERVER_H
