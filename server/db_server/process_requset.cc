//
// Created by yqt on 18-11-9.
//


#include "process_requset.h"
#include "base/ImPdu.h"
#include "base/UniqueId.h"
#include "MySqlHandler.h"
#include "proto/IMCmdIdDefine.pb.h"


bool process_requset::ProcessPDU(std::shared_ptr<CImPdu> ppdu, std::shared_ptr<ITaskDB> ptr) {
    if (ppdu->GetServiceId() == IM::CmdIdDefine::SID_CONN) {
        if (ppdu->GetCommandId() == IM::CmdIdDefine::cmd_login_req) {
            ptr == make_shared<DBLoginTask>();
        } else if (ppdu->GetCommandId() == IM::CmdIdDefine::cmd_register_req){

        }
    }
   else if (ppdu->GetServiceId() == IM::CmdIdDefine::SID_MSG)
   {
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
