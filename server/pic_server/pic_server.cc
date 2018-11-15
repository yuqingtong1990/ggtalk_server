//
// Created by yqt on 18-11-14.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include "json/json.h"
#include "pic_server.h"
#include "evpp/httpc/request.h"
#include "evpp/httpc/response.h"
#include "evpp/httpc/url_parser.h"

using namespace std;

bool PicServer::Start(evpp::EventLoop *loop, short port) {

    evpp::http::Server server(10);
    server.SetThreadDispatchPolicy(evpp::ThreadDispatchPolicy::kIPAddressHashing);

    server.RegisterDefaultHandler(std::bind(&PicServer::DefaultHandler,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server.RegisterHandler("/getDefaultAvatar", std::bind(&PicServer::getDefailtAvaterHandler,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server.RegisterHandler("/uploadAvater", std::bind(&PicServer::UploadAvaterHandler,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
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

void PicServer::getDefailtAvaterHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                                        const evpp::http::HTTPSendResponseCallback &cb) {
    LOG_INFO << "usr ip is "<<ctx->remote_ip();
    ctx->AddResponseHeader("Content-Type", "application/x-png");
    ctx->AddResponseHeader("Content-Disposition","filename=default_avatar.png");
    ctx->AddResponseHeader("Access-Control-Allow-Origin", "*");
    ctx->AddResponseHeader("Server", "evpp");

    std::ifstream t("/tmp/default_avatar.png");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string contents(buffer.str());
    cb(contents);
}

void PicServer::UploadAvaterHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                                    const evpp::http::HTTPSendResponseCallback &cb) {
    LOG_INFO<<ctx->remote_ip()<<endl;
    LOG_INFO<<ctx->body().size()<<endl;
    std::string strContentType = ctx->FindRequestHeader("Content-Type");
    //LOG_INFO<<strContentType<<endl;
    size_t npos = strContentType.find("boundary=",0);
    std::string strkey = strContentType.substr(npos+9);
    LOG_INFO<<strkey<<endl;

    std::string strStart =  "--"+ strkey+;
    std::string strEnd = "--"+strkey+"--";

    LOG_INFO<< strStart << endl;
    LOG_INFO<< strEnd << endl;
    std::string body = ctx->body().ToString();
    //LOG_INFO<< body << endl;
    size_t nstart = body.find(strStart)+strStart.size();
    size_t nEnd = body.find(strEnd,nstart);
    LOG_INFO<<"start: "<<nstart <<" "<<"end: "<<nEnd<<endl;

    string strImg = body.substr(nstart,nEnd-strStart.size());

    nstart = strImg.find("\r\n\r\n")+4;
    nEnd = strImg.find("\r\n\r\n",nstart);

    strImg = strImg.substr(nstart,nEnd);
    LOG_INFO<<strImg<<endl;

    fstream f("/tmp/1.jpeg", ios::out);
    f << strImg;//写入数据
    f.close();

    Json::Value root;
    root["success"] = true;
    cb(root.toStyledString());
}


