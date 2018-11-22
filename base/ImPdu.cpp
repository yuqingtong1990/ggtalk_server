#include "ImPdu.h"

bool CImPdu::IsPduAvailable(char* buf, uint32_t len, uint32_t& pdu_len)
{
	if (len < sizeof(PduHeader_t))
		return false;

	CByteStream is(buf, len);
	is >> pdu_len;
	if (pdu_len > len)
	{
		//log("pdu_len=%d, len=%d\n", pdu_len, len);
		return false;
	}
// 
// 	if (0 == pdu_len)
// 	{
// 		throw CPduException(1, "pdu_len is 0");
// 	}

	return true;
}

CImPdu* CImPdu::ReadPdu(char *buf, uint32_t len)
{
	uint32_t pdu_len = 0;
	if (!IsPduAvailable(buf, len, pdu_len))
		return nullptr;

	CImPdu* pPdu = nullptr;

	pPdu = new CImPdu();
	pPdu->Write(buf, pdu_len);
	pPdu->ReadPduHeader(buf, sizeof(PduHeader_t));

	return pPdu;
}


CImPdu::CImPdu()
{
	memset(&header_, 0, sizeof(PduHeader_t));
}

char* CImPdu::GetBuffer()
{
	return buf_.GetBuffer();
}

uint32_t CImPdu::GetLength()
{
	return buf_.GetWriteOffset();
}

char* CImPdu::GetBodyData()
{
	return buf_.GetBuffer() + sizeof(PduHeader_t);
}

uint32_t CImPdu::GetBodyLength()
{
	uint32_t body_length = 0;
	body_length = buf_.GetWriteOffset() - sizeof(PduHeader_t);
	return body_length;
}

bool CImPdu::ReadPduHeader(char* buf, uint32_t len)
{
	bool ret = false;
	if (len >= sizeof(PduHeader_t) && buf) {
		CByteStream is(buf, len);

		is >> header_.length;
		is >> header_.version;
		is >> header_.flag;
		is >> header_.service_id;
		is >> header_.command_id;
		is >> header_.seq_num;
		is >> header_.usrid;
		is >> header_.reversed;
		ret = true;
	}
	return ret;
}

void CImPdu::SetPBMsg(const google::protobuf::MessageLite* msg)
{
	buf_.Read(NULL, buf_.GetWriteOffset());
	buf_.Write(NULL, sizeof(PduHeader_t));
	uint32_t msg_size = static_cast<uint32_t>(msg->ByteSize());
	char* szData = new (std::nothrow)char[msg_size];
	if(nullptr == szData)
		return;
	memset(szData,0,msg_size);

	if (!msg->SerializeToArray(szData, msg_size))
	{
		//LOG_DEBUG <<"pb msg miss required fields.";
	}

	buf_.Write(szData, msg_size);
	delete[]szData;
	WriteHeader();
}

void CImPdu::WriteHeader()
{
	char* buf = GetBuffer();
	header_.length = GetLength();
	CByteStream::WriteUint32(buf, header_.length);
	CByteStream::WriteUint16(buf + 4, header_.version);
	CByteStream::WriteUint16(buf + 6, header_.flag);
	CByteStream::WriteUint16(buf + 8, header_.service_id);
	CByteStream::WriteUint16(buf + 10, header_.command_id);
	CByteStream::WriteUint16(buf + 12, header_.seq_num);
	CByteStream::WriteUint32(buf + 12, header_.usrid);
	CByteStream::WriteUint32(buf + 14, header_.reversed);
}


