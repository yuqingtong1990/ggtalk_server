//
// Created by yqt on 18-11-19.
//

#ifndef ALL_CLIENTSESSION_H
#define ALL_CLIENTSESSION_H

#include "evpp/tcp_server.h"
#include "evpp/buffer.h"
#include "evpp/tcp_conn.h"
#include "evpp/event_loop.h"
#include "evpp/tcp_client.h"
#include "evpp/timestamp.h"
#include "ImPdu.h"

enum clientType
{
	unknowtype = 0,	//未知
	pc = 1,			//pc客户端
	mac = 2,		//mac客户端
	android = 3,	//Android客户端
	iphone = 4,		//iphone客户端
	ipad = 5,		//ipad客户端
	web = 6,		//web端(实际来自restapi)
};

struct UserInfo
{
	int32_t     userid;
	std::string username;
	std::string nickname;
	std::string password;
	int32_t     clienttype;
	int32_t     status;
};

enum ManyOnlineType
{
	NoManyOnline = 1, //没有多端在线
	PcMobileOnline,//移动端 pc端同时在线
	AllManyOnline, //不同的端都可以同时在线
};

class ClientSession
    :public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(const std::shared_ptr<evpp::TCPConn>& conn);
	virtual ~ClientSession() = default;

    ClientSession(const ClientSession& rhs) = delete;
	ClientSession& operator =(const ClientSession& rhs) = delete;

    std::shared_ptr<evpp::TCPConn> GetConnectionPtr(){return tmpConn_.lock();}
    void OnClinetMessage(const evpp::TCPConnPtr& conn, evpp::Buffer* msg);

	void Send(const void* data, size_t len);
	uint64_t getConnId() { return connect_id_; }
	int32_t getUserId() { return user_id_; }
	int64_t getSessionId(ManyOnlineType type);
	static int64_t getSessionId(ManyOnlineType type,int32_t userid,int32_t clienttype);
	void OnReply(CImPdu* pdu);
	void ForcedOffline();

private:
    bool ProcessBuffer(const evpp::TCPConnPtr& conn, const std::string& strbuf);
	bool ProcessPDU(CImPdu* ppdu, const evpp::TCPConnPtr& conn);

	std::weak_ptr<evpp::TCPConn> tmpConn_;
	evpp::Timestamp	timestamp_last;
	std::string  strAesKey;
	uint64_t	connect_id_ = 0;

	int32_t     clienttype_ = 0;
	int32_t     status = 0;

	int32_t      user_id_ = 0;
	std::string  user_name_;
	std::string  user_nickname_;
};


#endif //ALL_CLIENTSESSION_H
