//
// Created by yqt on 18-11-14.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <base/StringUtil.h>
#include "json/json.h"
#include "pic_server.h"
#include "evpp/httpc/request.h"
#include "evpp/httpc/response.h"
#include "evpp/httpc/url_parser.h"
#include "httpformdata.h"
#include "fileHandle.h"

using namespace std;

bool PicServer::Start(evpp::EventLoop *loop, short port) {
    makedir();
    evpp::http::Server server(10);
    server.SetThreadDispatchPolicy(evpp::ThreadDispatchPolicy::kIPAddressHashing);

    server.RegisterDefaultHandler(std::bind(&PicServer::DefaultHandler,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server.RegisterHandler("/getDefaultAvatar", httpHandler::getDefailtAvaterHandler);
    server.RegisterHandler("/uploadUserAvater", httpHandler::uploadUserAvaterHandler);
    server.RegisterHandler("/uploadChatImg", httpHandler::uploadChatImgHandler);
    server.RegisterHandler("/getUserAvaterImg", httpHandler::getUserAvaterImgHandler);
    server.Init(port);
    server.Start();
    return false;
}

void PicServer::Loop() {
    loop_->Run();
}

void PicServer::DefaultHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                               const evpp::http::HTTPSendResponseCallback &cb) {
    ctx->AddResponseHeader("Content-Type", "text/html");
    ctx->AddResponseHeader("Access-Control-Allow-Origin", "*");
    ctx->AddResponseHeader("Server", "evpp");

    std::ifstream t("/tmp/index.html");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string contents(buffer.str());
    cb(contents);
}

bool PicServer::makedir() {

    std::string rootDir = IncludeTrailingPathDelimiter(getHomePath())+"gg_talk/pic_srv/";
    if (!ForceDirectory(rootDir)){
        LOG_ERROR<<"create rootDir fail";
        return false;
    }

    std::string userAvater = rootDir+"/avater/user/";
     if (!ForceDirectory(userAvater)){
        LOG_ERROR<<"create userAvater Dir fail";
        return false;
    }

    std::string groupAvater = rootDir+"/avater/group/";
     if (!ForceDirectory(groupAvater)){
        LOG_ERROR<<"create groupAvater Dir fail";
        return false;
    }

     std::string chat = rootDir+"/chat/";
     if (!ForceDirectory(chat)){
        LOG_ERROR<<"create chat Dir fail";
        return false;
    }

    return false;
}
