//
// Created by yqt on 18-11-19.
//

#ifndef ALL_DBCONNECT_H
#define ALL_DBCONNECT_H

#include "evpp/tcp_conn.h"
#include "evpp/tcp_client.h"

class dbConnect {
public:
    dbConnect();
    ~dbConnect();
    bool Start(const char* ip = "0.0.0.0", short port =8100);
    void Loop();
    void OnConnection(const evpp::TCPConnPtr& conn);
    void OnDbReplyMessage(const evpp::TCPConnPtr& conn,evpp::Buffer* msg);
private:
    std::shared_ptr<evpp::TCPClient> client_;
    evpp::EventLoop loop_;
};


#endif //ALL_DBCONNECT_H
