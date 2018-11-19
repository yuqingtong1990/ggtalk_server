//
// Created by yqt on 18-11-19.
//

#ifndef ALL_HTTPHANDLER_H
#define ALL_HTTPHANDLER_H

#include "evpp/http/http_server.h"
#include "evpp/event_loop.h"

class httpHandler {
public:
    httpHandler() = default;
    ~httpHandler() = default;

public:
     //获取默认头像
    static void getDefailtAvaterHandler(evpp::EventLoop* loop,\
                  const evpp::http::ContextPtr& ctx,\
                  const evpp::http::HTTPSendResponseCallback& cb);

    //上传用户头像
    static void uploadUserAvaterHandler(evpp::EventLoop* loop,\
                  const evpp::http::ContextPtr& ctx,\
                  const evpp::http::HTTPSendResponseCallback& cb);

    //上传聊天图片(允许批量上传)
    static void uploadChatImgHandler(evpp::EventLoop* loop,\
                  const evpp::http::ContextPtr& ctx,\
                  const evpp::http::HTTPSendResponseCallback& cb);

    //获取用户头像
    static void getUserAvaterImgHandler(evpp::EventLoop* loop,\
                  const evpp::http::ContextPtr& ctx,\
                  const evpp::http::HTTPSendResponseCallback& cb);
};


#endif //ALL_HTTPHANDLER_H
