//
// Created by yqt on 18-11-19.
//

#include "ClientSession.h"
#include "UniqueId.h"

#define MAKE_I64(hi, lo) ((int64_t(int32_t(hi) & 0xffffffff) << 32 ) | int64_t(int32_t(lo) & 0xffffffff))


ClientSession::ClientSession(const std::shared_ptr<evpp::TCPConn> &conn)
:tmpConn_(conn)
{
    if (conn)
		connect_id_ = conn->id();
}


void ClientSession::OnClinetMessage(const evpp::TCPConnPtr &conn, evpp::Buffer *msg) {
    std::string sMsg = msg->NextAllString();
	LOG_DEBUG << conn->remote_addr() << ":" << sMsg;
	ProcessBuffer(conn, sMsg);
}

void ClientSession::Send(const void *data, size_t len) {
    if (GetConnectionPtr() != nullptr)
	{
		GetConnectionPtr()->Send(data, len);
	}
}

int64_t ClientSession::getSessionId(ManyOnlineType type) {
    return getSessionId(type, user_id_, clienttype_);
}

int64_t ClientSession::getSessionId(ManyOnlineType type, int32_t userid, int32_t clienttype) {
    if (NoManyOnline == type)
	{
		return MAKE_I64(0, userid);
	}

    if (PcMobileOnline == type)
	{
		if (clienttype == pc || clienttype == mac)
		{
			return MAKE_I64(0, userid);
		}
        if (clienttype == android || clienttype == iphone || clienttype == ipad)
		{
			return MAKE_I64(1, userid);
		}
        return MAKE_I64(2, userid);

	}

	if (AllManyOnline == type)
	{
		return MAKE_I64(clienttype, userid);
	}
}

void ClientSession::OnReply(CImPdu *pdu) {

}

void ClientSession::ForcedOffline() {

    user_id_ = 0;
	user_name_.clear();
	user_nickname_.clear();

}

bool ClientSession::ProcessBuffer(const evpp::TCPConnPtr &conn, const std::string &strbuf) {
    CImPdu* ppdu = nullptr;
	do
	{
		if (strbuf.empty())
			break;

		uint64_t id = 0;
		ppdu = CImPdu::ReadPdu((char*)strbuf.c_str(), static_cast<uint32_t>(strbuf.size()));
		if (ppdu == nullptr){
			conn->Close();
			break;
		}

		if (!ProcessPDU(ppdu, conn)){
			break;
		}
		//需要抛到数据代理服务器处理
		timestamp_last = evpp::Timestamp::Now();
	} while (0);

	if (ppdu != nullptr)
		delete ppdu;

	return true;
}

bool ClientSession::ProcessPDU(CImPdu *ppdu, const evpp::TCPConnPtr &conn) {
    return false;
}
