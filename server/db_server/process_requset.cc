//
// Created by yqt on 18-11-9.
//

#include "process_requset.h"
#include "base/ImPdu.h"
#include "base/UniqueId.h"
#include "proto/IMBaseDefine.pb.h"
#include "proto/IMCmdIdDefine.pb.h"
#include "proto/IMCommPackage.pb.h"
#include "proto/IMFriendPackage.pb.h"
#include "proto/IMMsgPackage.pb.h"

bool process_requset::ProcessBuffer(uint64_t conn_id, const std::string &strbuf) {
    CImPdu* ppdu = nullptr;
    do
    {
        if (strbuf.empty())
            break;

        ppdu = CImPdu::ReadPdu((char*)strbuf.c_str(), strbuf.size());
        if (ppdu == nullptr){
            break;
        }

        if (!ProcessPDU(conn_id,ppdu)){
            break;
        }
        /*
        if (!createDBtask(ppdu, shared_from_this(),id)){
            //conn->Close();
            break;
        }
         */
    } while (0);

    if (ppdu)
        delete ppdu;

    return true;
}

bool process_requset::ProcessPDU(uint64_t conn_id, CImPdu *ppdu) {
    if (ppdu->GetServiceId() == IM::CmdIdDefine::SID_CONN) {
        if (ppdu->GetCommandId() == IM::CmdIdDefine::cmd_login_req) {

        } else if (ppdu->GetCommandId() == IM::CmdIdDefine::cmd_register_req){

        }
    } else if (ppdu->GetServiceId() == IM::CmdIdDefine::SID_MSG){
        if (ppdu->GetCommandId() == IM::CmdIdDefine::cmd_message_sendmsg_req){
            //插入mongodb数据库，并不需要回复消息服务器，判断是否要插入离线消息表
        }
        else if(ppdu->GetCommandId() == IM::CmdIdDefine::cmd_message_getmessage_req){
            //获取离线消息，并回复给消息服务器
        }
    } else if (ppdu->GetServiceId() == IM::CmdIdDefine::SID_GROUP){

    }

    return true;
}
