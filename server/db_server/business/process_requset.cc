//
// Created by yqt on 18-11-9.
//

#include "process_requset.h"

#include "proto/IMBaseDefine.pb.h"
#include "proto/IMCmdIdDefine.pb.h"
#include "proto/IMCommPackage.pb.h"
#include "proto/IMFriendPackage.pb.h"
#include "proto/IMMsgPackage.pb.h"

bool process_requset::ProcessBuffer(uint64_t id, const std::string &strbuf) {
    return false;
}

bool process_requset::ProcessPDU(uint64_t id, CImPdu *ppdu) {
    return false;
}
