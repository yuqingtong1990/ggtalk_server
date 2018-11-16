#include "UniqueId.h"
#include "Snowflake.h"
#include <stdio.h>

#ifdef WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#include <cstring>
#include <iostream>

#endif

typedef struct _GUID {
uint32_t Data1;
uint16_t Data2;
uint16_t Data3;
uint8_t Data4[8];
} GUID;

/*
 * windows下需要引用 ole32.lib 在linux下编译时需要链接uuid库（-luuid）
 * */

GUID CreateGuid()
{
	GUID guid;
#ifdef WIN32
	CoCreateGuid(&guid);
#else
	uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif
	return guid;
}

std::string GuidToString(const GUID &guid){
	 char buf[64] = {0};
#ifdef __GNUC__
    snprintf(
#else // MSVC
    _snprintf_s(
#endif
	buf,sizeof(buf),"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			 guid.Data1, guid.Data2, guid.Data3,
             guid.Data4[0], guid.Data4[1],
             guid.Data4[2], guid.Data4[3],
             guid.Data4[4], guid.Data4[5],
             guid.Data4[6], guid.Data4[7]
	);
    return std::string(buf);
}

std::string CreateGuidStr() {
    GuidToString(CreateGuid());
}


static SnowflakeIdWorker s_worker;

uint64_t sfUniqueId(uint32_t datacenterId, uint32_t workid) {

    if (datacenterId > 31){
        datacenterId = datacenterId%31;
    }

    if (workid > 31){
        workid = workid%31;
    }
    s_worker.setDatacenterId(datacenterId);
	s_worker.setWorkerId(workid);
    return s_worker.getId();
}



const char* strNum = "0123456789";
const char* strCapital= "abcdefghijklmnopqrstuvwxyz";
const char* strLowercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";



#define random(x) (rand()%x)

std::string CreateRandomStr(uint32_t len) {

    char* ch =  new(std::nothrow)char[len+1];
    memset(ch,0,len+1);
    std::cout<<len<<std::endl;
    if (ch == nullptr)
        return "";
    srand(static_cast<unsigned int>((int)time(nullptr)));
    const char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < len; ++i) {
        ch[i] = CCH[random(sizeof(CCH) - 1)];
    }
    return std::string(ch,len);
}

