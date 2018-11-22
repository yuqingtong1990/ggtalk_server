//
// Created by yqt on 18-11-8.
//

#include "db_server.h"
#include "dbWrapper/MongoAsynMgr.h"
#include "process_requset.h"

using namespace evpp;

DBServer::DBServer() {
    isRun_.store(false);
}

DBServer::~DBServer() {

}

void DBServer::OnConnection(const evpp::TCPConnPtr &conn) {

    LOG_INFO << conn->AddrToString() << " is " << (conn->IsConnected() ? "UP" : "DOWN");
    if (conn->IsConnected())
    {
        LOG_INFO << "A new connection from " << conn->remote_addr();
        std::lock_guard<std::mutex> guard(mutex_conn_);
        mapIdConn_[conn->id()] = conn;
    } else {
        OnClose(conn);
        LOG_INFO << "Lost the connection from " << conn->remote_addr();
    }
}

std::shared_ptr<CImPdu> generatePdu(char *buf, uint32_t len){
    uint32_t pdu_len = 0;
	if (!CImPdu::IsPduAvailable(buf, len, pdu_len))
		return nullptr;
    std::shared_ptr<CImPdu> ptr = make_shared<CImPdu>();
    ptr->Write(buf,len);
    ptr->ReadPduHeader(buf,sizeof(PduHeader_t));
    return ptr;

}

void DBServer::OnChatMessage(const evpp::TCPConnPtr &conn, evpp::Buffer *msg) {
    //在这里接收消息服务器过来的消息
    std::string sMsg = msg->NextAllString();
	LOG_DEBUG << "ip: "<< conn->remote_addr() <<"id:" << conn->id() << "data: " << sMsg;

    std::shared_ptr<CImPdu> ppdu = generatePdu((char*)sMsg.c_str(), static_cast<uint32_t>(sMsg.size()));
    if (ppdu == nullptr){

        LOG_WARN << "msg server send a error pack";
        return;
    }

    std::shared_ptr<ITaskDB> ptrTask;
    if (!process_requset::getSingletonPtr()->ProcessPDU(ppdu,ptrTask)){
        return;
    }

    if (ptrTask){
        ptrTask->SetId(conn->id());
        ptrTask->SetImPdu(ppdu);
        ptrTask->SetDataCallBack(std::bind(&DBServer::CallBackPdu,this,std::placeholders::_1, std::placeholders::_2));
        TaskDBThdPool::getSingletonPtr()->AddReq(ptrTask);
    }

}

bool DBServer::Start(evpp::EventLoop *loop, const char *ip, short port) {
    bool bRet = false;
    if (loop == nullptr)
        return false;

    char ch[100] = { 0 };
    snprintf(ch, 100, "%s:%d", ip, port);
    tcpserver_.reset(new TCPServer(loop, ch, "chatserver", 10));
    bRet = tcpserver_->Init();
    if (!bRet)
    {
        LOG_ERROR << "chatserver bind fail";
        return false;
    }

    tcpserver_->SetConnectionCallback(std::bind(&DBServer::OnConnection, this, std::placeholders::_1));
	tcpserver_->SetMessageCallback(std::bind(&DBServer::OnChatMessage, this, std::placeholders::_1, std::placeholders::_2));
    bRet = tcpserver_->Start();
    if (!bRet)
    {
        LOG_ERROR << "chatserver bind fail";
        return false;
    }
     isRun_ = true;
    return true;
}

void DBServer::Loop() {
    loop_->Run();
}

void DBServer::OnClose(const evpp::TCPConnPtr &conn) {
    std::lock_guard<std::mutex> guard(mutex_conn_);

    if (mapIdConn_.size())
        return;

    auto iter = mapIdConn_.find(conn->id());
    if (iter != mapIdConn_.end()){
        mapIdConn_.erase(iter++);
    }
}

bool DBServer::Init(std::string &strErrMsg) {
    return false;
}

void DBServer::End() {
    MongoWrapper::shutdown();
}

bool DBServer::InitRedis() {
    return false;
}

bool DBServer::InitMongoDbMgr() {
    return false;
}

bool DBServer::InitMysqlDbMgr() {
    return false;
}

void DBServer::CallBackPdu(uint64_t id, std::shared_ptr<CImPdu> ppdu) {

}
