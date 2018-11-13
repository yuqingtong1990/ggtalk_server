//
// Created by yqt on 18-11-12.
//

#ifndef ALL_MESSAGEDBCONTROL_H
#define ALL_MESSAGEDBCONTROL_H

#include <string>
#include "MongoAsynMgr.h"

class MessageDBControl {
public:
    MessageDBControl() = default;
    ~MessageDBControl() = default;

    bool Init();
    /*
     * 插入用户消息
     * sessionid smalluId & biguId(因为A与B的聊天记录和B与A的聊天记录是一样的)
     * content 消息内容
     * offlineMsg 是否插入离线消息表(消息服务器如果没有转发则插入离线消息表里)
     * 是否插入离线消息
    */
    void InsertUserMessage(uint64_t sessionid,const std::string& content);

    /*
     * 插入群组消息
     * groupId
     * content 消息内容
     * offlineMsg 是否插入离线消息表
     * 是否插入离线消息
    */
    void InsertGroupMessage(uint32_t groupId,const std::string& content);

    /*
     * 插入离线消息
     * userId:用户id
     * content 消息内容
     * offlineMsg 是否插入离线消息表
     * 是否插入离线消息
    */
    void InsertOffLineMsg(uint32_t userId,const std::string& content);
    /*
     *获取某个用户离线消息
     *userId:用户id
     *bHaveRead:是否算已读，如果为true就从数据库中删除文件
     */
    void GetUserOffLineMsg(uint32_t userId,bool bHaveRead);
    /*
     *将某个用户对某人的消息标记为已读
     * */
    void SetMsgHaveRead(uint32_t userId,uint32_t peerId);

    //回调函数
    void InsertUserMessage_callback(const ResultMap& ret);
    void InsertGroupMessage_callback(const ResultMap& ret);
    void InsertOfflineMsg_callback(const ResultMap& ret);
    void GetUserOffLineMsg_callback(const ResultMap& ret);
    void SetMsgHaveRead_callback(const ResultMap& ret);
};


#endif //ALL_MESSAGEDBCONTROL_H
