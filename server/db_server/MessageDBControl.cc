//
// Created by yqt on 18-11-12.
//
#include <json/value.h>
#include "DbSrvConfigMgr.h"
#include "MessageDBControl.h"
#include "evpp/logging.h"
#include "DbSrvConfigMgr.h"

bool MessageDBControl::Init() {
    MongoWrapper::initMongodb();
    //初始化3个数据库
    MongoAsynMgr::getSingletonPtr()->addConnect("usrchat_record",ReadMode::k_primary,\
                                                mongoDbCofig::getSingletonPtr()->getChatRecord().host_,\
                                                mongoDbCofig::getSingletonPtr()->getChatRecord().replset_,\
                                                mongoDbCofig::getSingletonPtr()->getChatRecord().dbname_,\
                                                mongoDbCofig::getSingletonPtr()->getChatRecord().user_,\
                                                mongoDbCofig::getSingletonPtr()->getChatRecord().passwd_);
    MongoAsynMgr::getSingletonPtr()->addConnect("group_record",ReadMode::k_primary,\
                                                mongoDbCofig::getSingletonPtr()->getGroupRecord().host_,\
                                                mongoDbCofig::getSingletonPtr()->getGroupRecord().replset_,\
                                                mongoDbCofig::getSingletonPtr()->getGroupRecord().dbname_,\
                                                mongoDbCofig::getSingletonPtr()->getGroupRecord().user_,\
                                                mongoDbCofig::getSingletonPtr()->getGroupRecord().passwd_);

    MongoAsynMgr::getSingletonPtr()->addConnect("offline_Msg",ReadMode::k_primary,\
                                                mongoDbCofig::getSingletonPtr()->getOfflineMsg().host_,\
                                                mongoDbCofig::getSingletonPtr()->getOfflineMsg().replset_,\
                                                mongoDbCofig::getSingletonPtr()->getOfflineMsg().dbname_,\
                                                mongoDbCofig::getSingletonPtr()->getOfflineMsg().user_,\
                                                mongoDbCofig::getSingletonPtr()->getOfflineMsg().passwd_);
    MongoAsynMgr::getSingletonPtr()->start();

    return true;
}

void MessageDBControl::InsertUserMessage(uint64_t sessionid, const std::string &content) {

    std::shared_ptr<MongoAsynOper> optr;
    optr = std::make_shared<MongoAsynOper>(std::bind(&MessageDBControl::InsertUserMessage_callback, this,std::placeholders::_1));
    char ch[100] = {0};

    sprintf(ch,"u_%llu",sessionid);
    optr->insertMany(ch,content);
    MongoAsynMgr::getSingletonPtr()->runOper("usrchat_record", optr);
}

void MessageDBControl::InsertGroupMessage(uint32_t groupId, const std::string &content) {
    std::shared_ptr<MongoAsynOper> optr;
    optr = std::make_shared<MongoAsynOper>(std::bind(&MessageDBControl::InsertUserMessage_callback, this,std::placeholders::_1));
    char ch[100] = {0};

    sprintf(ch,"g_%u",groupId);
    optr->insertMany(ch,content);
    MongoAsynMgr::getSingletonPtr()->runOper("group_record", optr);
}

void MessageDBControl::InsertOffLineMsg(uint32_t userId, const std::string &content) {
    std::shared_ptr<MongoAsynOper> optr;
    optr = std::make_shared<MongoAsynOper>(std::bind(&MessageDBControl::InsertOfflineMsg_callback, this,std::placeholders::_1));
    char ch[100] = {0};
    sprintf(ch,"u_%u",userId);
    optr->insertMany(ch,content);
    MongoAsynMgr::getSingletonPtr()->runOper("offline_Msg", optr);
}

void MessageDBControl::GetUserOffLineMsg(uint32_t userId,bool bHaveRead) {

    //这里是以后需要优化，如果离线消息过多，需要分次获取
    std::shared_ptr<MongoAsynOper> optr;
    optr = std::make_shared<MongoAsynOper>(std::bind(&MessageDBControl::InsertOfflineMsg_callback, this,std::placeholders::_1));
    char ch[100] = {0};
    sprintf(ch,"u_%u",userId);
    optr->selectMany(ch,"{}","{}","{time:-1}");
    if (bHaveRead){
        optr->drop(ch);
    }
    MongoAsynMgr::getSingletonPtr()->runOper("offline_Msg", optr);
}

void MessageDBControl::SetMsgHaveRead(uint32_t userId, uint32_t peerId) {
    std::shared_ptr<MongoAsynOper> optr;
    optr = std::make_shared<MongoAsynOper>(std::bind(&MessageDBControl::InsertOfflineMsg_callback, this,std::placeholders::_1));
    char ch[100] = {0};
    sprintf(ch,"u_%u",userId);
    Json::Value query;
    query["receiver"] = peerId;
    optr->deleteMany(ch,query.toStyledString());
    MongoAsynMgr::getSingletonPtr()->runOper("offline_Msg", optr);
}

void MessageDBControl::InsertUserMessage_callback(const ResultMap &ret) {
    for (auto&& iter = ret.begin(); iter != ret.end(); ++iter){
        LOG_INFO << "operId:" << iter->second->id << "code "<< iter->second->code <<"count" << iter->second->count;
    }
}

void MessageDBControl::InsertGroupMessage_callback(const ResultMap &ret) {
    for (auto&& iter = ret.begin(); iter != ret.end(); ++iter){
           LOG_INFO << "operId:" << iter->second->id << "code "<< iter->second->code <<"count" << iter->second->count;
    }
}

void MessageDBControl::InsertOfflineMsg_callback(const ResultMap &ret) {
    for (auto&& iter = ret.begin(); iter != ret.end(); ++iter){
            LOG_INFO << "operId:" << iter->second->id << "code "<< iter->second->code <<"count" << iter->second->count;
    }
}

void MessageDBControl::GetUserOffLineMsg_callback(const ResultMap &ret) {
     for (auto&& iter = ret.begin(); iter != ret.end(); ++iter){
         LOG_INFO << "operId:" << iter->second->id << "code "<< iter->second->code <<"count" << iter->second->count;
         if (iter->second->id == MongoDataOper::op_selectMany){
            //修改代码

         }
         else if (iter->second->id == MongoDataOper::op_drop){

         }

    }
}

void MessageDBControl::SetMsgHaveRead_callback(const ResultMap &ret) {
     for (auto&& iter = ret.begin(); iter != ret.end(); ++iter) {
         LOG_INFO << "operId:" << iter->second->id << "code " << iter->second->code << "count" << iter->second->count;
          if (iter->second->id == MongoDataOper::op_deleteMany){
            //修改代码

         }
     }
}


