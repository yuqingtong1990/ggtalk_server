//
// Created by yqt on 18-11-22.
//
#include <sstream>
#include <evpp/logging.h>
#include "proto/IMCommPackage.pb.h"
#include "MySqlHandler.h"
#include "proto/IMBaseDefine.pb.h"
#include "proto/IMCmdIdDefine.pb.h"

bool DBLoginTask::isCellphone(const std::string &ss) {
    for (char s : ss) {
		if (isdigit(s) == 0)
			return false;
	}
	return true;
}

bool DBLoginTask::isEmail(const std::string &ss) {
   return ss.find('@') != std::string::npos;
}

void DBLoginTask::Process(std::shared_ptr<CDatabaseMysql> poConn) {

    QueryResult* pResult = nullptr;
    IM::CommPackage::IMUsrLoginRsp rsp;

    do{

         if (ppdu_ == nullptr){
            rsp.set_success(false);
            rsp.set_errormsg("data is illegal");
            LOG_ERROR << "pdu is null,please check";
            break;
        }

        if (ppdu_->GetServiceId() != IM::CmdIdDefine::SID_CONN || ppdu_->GetCommandId() == IM::CmdIdDefine::cmd_login_req) {
            rsp.set_success(false);
            rsp.set_errormsg("data is illegal");
            LOG_ERROR << "pdu is type is error please check";
            break;
        }

        IM::CommPackage::IMUsrLoginReq req;
        if (!req.ParseFromArray(ppdu_->GetBodyData(), ppdu_->GetBodyLength())) {
            LOG_ERROR << "pdu parse is error , task is login";
            rsp.set_success(false);
            rsp.set_errormsg("data is illegal");
            break;
        }

        if (req.info().loginname().empty() || req.info().password().empty()){
            LOG_ERROR << "pdu parse is error , task is login";
            rsp.set_success(false);
            rsp.set_errormsg("loginname or password is error");
            break;
        }
        const char* psql_login_cellphone = "select* from ImUser where cellphone = \'%s\'";
	    const char* psql_login_email = "select* from ImUser where email = \'%s\'";
	    const char* psql_login_username = "select* from ImUser where name = \'%s\'";
	    char chsql[4096] = { 0 };
        do
        {
            if (isCellphone(req.info().loginname())) {
                snprintf(chsql, 4096, psql_login_cellphone, req.info().loginname().c_str());
                break;
            }

            if (isEmail(req.info().loginname())) {
                snprintf(chsql, 4096, psql_login_email, req.info().loginname().c_str());
                break;
            }

            snprintf(chsql, 4096, psql_login_username, req.info().loginname().c_str());
        } while (0);

        pResult = poConn->Query(chsql);

        if (pResult == nullptr)
        {
            //数据库记录查询不到
            rsp.set_success(false);
            rsp.set_errormsg("can not find user");
            break;
        }

        if (pResult->GetRowCount() != 1)
        {
            //数据库记录查询不到
            rsp.set_success(false);
            rsp.set_errormsg("can not find user");
            break;
        }

        Field* pRow = pResult->Fetch();
        if (pRow == nullptr)
        {
            rsp.set_success(false);
            rsp.set_errormsg("can not find user");
            break;
        }

        std::string strpassword = pRow[5].GetString();
        if (strcmp(strpassword.c_str(), req.info().password().c_str()) != 0)
        {
            rsp.set_success(false);
            rsp.set_errormsg("password is error");
            break;
        }

        auto * info = new IM::BaseDefine::UserDetailInfo();
        info->set_user_id(pRow[0].GetUInt32());
        info->set_user_cellphone(pRow[1].GetString());
        info->set_user_email(pRow[2].GetString());
        info->set_user_name(pRow[3].GetString());
        info->set_user_nick_name(pRow[4].GetString());
        info->set_user_gender(pRow[6].GetUInt8());
        info->set_user_avatar_url(pRow[7].GetString());
        info->set_birthday(pRow[8].GetString());
        info->set_school(pRow[9].GetString());
        info->set_hometown(pRow[10].GetString());
        info->set_career(pRow[11].GetString());
        info->set_describe(pRow[12].GetString());

        rsp.set_allocated_userinfo(info);
        rsp.set_success(true);
    } while (0);

    if (pResult != nullptr)
		pResult->EndQuery();

    std::shared_ptr<CImPdu> prsp = make_shared<CImPdu>();
	prsp->SetCommandId(IM::CmdIdDefine::cmd_login_rsp);
	prsp->SetServiceId(IM::CmdIdDefine::SID_CONN);
	prsp->SetPBMsg(&rsp);

	if (cb_)
	{
		cb_(id_,prsp);
	}
}
