//
// Created by yqt on 18-11-19.
//

#include "dbConnect.h"

dbConnect::dbConnect() {


}

dbConnect::~dbConnect() {

}

bool dbConnect::Start(const char *ip, short port) {
    char ch[100] = { 0 };
    snprintf(ch, 100, "%s:%d", ip, port);
    client_.reset(new evpp::TCPClient(&loop_,ch,"dbServer_conns"));
    client_->SetConnectionCallback(std::bind(&dbConnect::OnConnection, this, std::placeholders::_1));
	client_->SetMessageCallback(std::bind(&dbConnect::OnDbReplyMessage, this, std::placeholders::_1, std::placeholders::_2));
    std::thread th(std::bind(&dbConnect::Loop,this));
    th.detach();
    client_->Connect();
    return true;
}

void dbConnect::Loop() {
    loop_.Run();
}

void dbConnect::OnConnection(const evpp::TCPConnPtr &conn) {
    if (conn->IsConnected())
    {
        LOG_INFO << "Connected to " << conn->remote_addr();
        conn->Send("hello");
    }
    else
    {
        conn->loop()->Stop();
    }
}

void dbConnect::OnDbReplyMessage(const evpp::TCPConnPtr &conn, evpp::Buffer *msg) {
     LOG_TRACE << "Receive a message [" << msg->ToString() << "]";
}
