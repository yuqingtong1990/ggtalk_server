//
// Created by yqt on 18-11-19.
//

#include "httpHandler.h"
#include <base/StringUtil.h>
#include <base/fileHandle.h>
#include "json/json.h"
#include "evpp/httpc/request.h"
#include "evpp/httpc/response.h"
#include "evpp/httpc/url_parser.h"
#include "httpformdata.h"
using namespace std;

void httpHandler::getDefailtAvaterHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                                          const evpp::http::HTTPSendResponseCallback &cb) {
    LOG_INFO<<"service:uploadUserAvater"<<"  client ip:"<<ctx->remote_ip()<<"   bodysize:"<<ctx->body().size()<<std::endl;
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

void httpHandler::uploadUserAvaterHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                                          const evpp::http::HTTPSendResponseCallback &cb) {
    LOG_INFO<<"service:uploadUserAvater"<<"  client ip:"<<ctx->remote_ip()<<"   bodysize:"<<ctx->body().size()<<std::endl;
    std::string strContentType = ctx->FindRequestHeader("Content-Type");
    LOG_INFO<<strContentType<<endl;
    size_t npos = strContentType.find("boundary=",0);
    std::string strboundary = strContentType.substr(npos+9);
    LOG_INFO<<strboundary<<endl;

    MultipartParser parser(strboundary.c_str());
    parser.ParseBody(ctx->body().ToString());
    //parser.Print();
    for (auto iter : parser.getform()) {

        std::string strName = MultipartParser::QneryFindFromDisposition(iter.first,"name");
        std::string strfileName = MultipartParser::QneryFindFromDisposition(iter.first,"filename");

        std::string path  = "/tmp/"+strName+ExtractExtensions(strfileName);
        std::cout<<path<<endl;
        //save file
        SaveBufferToFile(path,iter.second);
    }

    Json::Value root;
    root["success"] = true;
    cb(root.toStyledString());
}

void httpHandler::uploadChatImgHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                                       const evpp::http::HTTPSendResponseCallback &cb) {
    LOG_INFO<<"service:uploadChatImg"<<"  client ip:"<<ctx->remote_ip()<<"   bodysize:"<<ctx->body().size()<<std::endl;

}

void httpHandler::getUserAvaterImgHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                                          const evpp::http::HTTPSendResponseCallback &cb) {
    LOG_INFO<<"service:getUserAvaterImg"<<"  client ip:"<<ctx->remote_ip()<<"   bodysize:"<<ctx->body().size()<<std::endl;

}
