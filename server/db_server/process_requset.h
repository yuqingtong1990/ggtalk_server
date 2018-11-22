//
// Created by yqt on 18-11-9.
//

#ifndef ALL_PROCESS_REQUSET_H
#define ALL_PROCESS_REQUSET_H

#include <cstdint>
#include <string>
#include "dbWrapper/dbmysqltaskpool.h"
#include "ImPdu.h"
#include "base/Singleton.h"
/*
 * 处理请求一般是消息服务器传递过来的
*/

class process_requset
    :public Singleton<process_requset>

{
public:
    process_requset() = default;
    ~process_requset() = default;

    bool ProcessPDU(std::shared_ptr<CImPdu> pdu,std::shared_ptr<ITaskDB> ptr);
};


#endif //ALL_PROCESS_REQUSET_H
