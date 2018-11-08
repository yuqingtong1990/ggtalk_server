//
// Created by yqt on 18-11-8.
//

#include "db_server.h"

DBServer::DBServer() {

}

DBServer::~DBServer() {

}

void DBServer::OnConnection(const evpp::TCPConnPtr &conn) {

}

void DBServer::OnChatMessage(const evpp::TCPConnPtr &conn, evpp::Buffer *msg) {

}

bool DBServer::Start(evpp::EventLoop *loop, const char *ip, short port) {
    if (loop == nullptr)
        return false;


}

void DBServer::Loop() {

}

void DBServer::OnClose(const evpp::TCPConnPtr &conn) {

}
