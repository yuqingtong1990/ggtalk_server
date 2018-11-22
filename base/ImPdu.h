#ifndef IMPDUBASE_H_
#define IMPDUBASE_H_
#include <stdint.h>
#include <string>
#include "UtilPdu.h"
#include "google/protobuf/message_lite.h"

//////////////////////////////
typedef struct Protobuf_Data_Unit_Header
{
    uint32_t 	length;	
    uint16_t 	version;
    uint16_t	flag;
    uint16_t	service_id;
    uint16_t	command_id;
    uint16_t	seq_num;
	uint32_t 	usrid;
    uint32_t    reversed;
} PduHeader_t;

class CImPdu
{
public:
	static bool IsPduAvailable(char* buf, uint32_t len, uint32_t& pdu_len);
	static CImPdu* ReadPdu(char *buf, uint32_t len);

    CImPdu();
	~CImPdu() = default;

	char* GetBuffer();
	uint32_t GetLength();
	char* GetBodyData();
	uint32_t GetBodyLength();

	void SetVersion(uint16_t version) { header_.version = version; }
	void SetFlag(uint16_t flag) { header_.flag = flag; }
	void SetServiceId(uint16_t service_id) { header_.service_id = service_id; }
	void SetCommandId(uint16_t command_id) { header_.command_id = command_id; }
	void SetSeqNum(uint16_t seq_num) { header_.seq_num = seq_num; }
	void SetUsrId(uint32_t usrid) { header_.usrid = usrid; };
	void SetReversed(uint32_t reversed) { header_.reversed = reversed; };


	uint16_t GetVersion() { return header_.version; }
	uint16_t GetFlag() { return header_.flag; }
	uint16_t GetServiceId() { return header_.service_id; }
	uint16_t GetCommandId() { return header_.command_id; }
	uint16_t GetSeqNum() { return header_.seq_num; }
	uint64_t GetUsrId() { return header_.usrid;}
	uint64_t GetReversed() { return header_.reversed; }


	void Write(const char* buf, uint32_t len) { buf_.Write((void*)buf, len); }
	bool ReadPduHeader(char* buf, uint32_t len);
	void SetPBMsg(const google::protobuf::MessageLite* msg);
	void WriteHeader();
private:
	CSimpleBuffer buf_;
	PduHeader_t header_;
};

#endif /* IMPDUBASE_H_ */