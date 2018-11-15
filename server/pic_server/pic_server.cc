//
// Created by yqt on 18-11-14.
//
#include "json/json.h"
#include "pic_server.h"
#include "evpp/httpc/request.h"
#include "evpp/httpc/response.h"
#include "evpp/httpc/url_parser.h"


bool PicServer::Start(evpp::EventLoop *loop, short port) {

    evpp::http::Server server(10);
    server.SetThreadDispatchPolicy(evpp::ThreadDispatchPolicy::kIPAddressHashing);

    server.RegisterDefaultHandler(std::bind(&PicServer::DefaultHandler,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server.RegisterHandler("/getDefailtAvater", std::bind(&PicServer::ProxyHandler,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server.Init(port);
    server.Start();
    return false;
}

void PicServer::Loop() {
    loop_->Run();
}

void PicServer::ProxyHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                             const evpp::http::HTTPSendResponseCallback &cb) {
    std::string uri = ctx->original_uri();
    size_t index = uri.find('?');
    if (index == std::string::npos) {
        cb("Format error. Please try again. The valid format is [http://127.0.0.1:9099/proxy?http://www.so.com/robots.txt]");
        return;
    }

    std::string proxy_url = uri.substr(index + 1);
    std::shared_ptr<evpp::httpc::GetRequest> r(new evpp::httpc::GetRequest(loop, proxy_url, evpp::Duration(3.0)));
    auto f = [cb, ctx, r](const std::shared_ptr<evpp::httpc::Response>& response) {
        ctx->set_response_http_code(response->http_code());
        std::string rc = response->body().ToString();
        cb(rc);
    };
    r->Execute(f);
}

void PicServer::DefaultHandler(evpp::EventLoop *loop, const evpp::http::ContextPtr &ctx,
                               const evpp::http::HTTPSendResponseCallback &cb) {
    Json::Value root;
    root["time"] = evpp::Timestamp::Now().Unix();

    Json::Value array;
    root["function"] = array;
    Json::Value value;
    value["getDefailtAvater"] = "获取默认头像";
    array.append(value);


    root["funtion"] = array;
    ctx->AddResponseHeader("Content-Type", "application/json");
    ctx->AddResponseHeader("Access-Control-Allow-Origin", "*");
    ctx->AddResponseHeader("Server", "evpp");
    cb(root.toStyledString());
}


