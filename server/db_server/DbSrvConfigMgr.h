//
// Created by yqt on 18-11-9.
//

#ifndef ALL_DBSRVCONFIGMGR_H
#define ALL_DBSRVCONFIGMGR_H

#include <string>
#include "Singleton.h"

class mysqlDbConfig
    :public Singleton<mysqlDbConfig>
{
public:
    mysqlDbConfig() = default;
    ~mysqlDbConfig() = default;

    void Set(const std::string& host, const std::string& user, const std::string& password, const std::string& dbname);
	bool LoadCfgFromFile();

    std::string& host();
	std::string& user();
	std::string& password();
	std::string& dbname();
private:
	std::string host_;
	std::string user_;
	std::string password_;
	std::string dbname_;
};

struct cfgMongo{
	std::string host_ = "localhost:27017";
	std::string dbname_;
	std::string replset_;
	std::string user_;
	std::string passwd_;
};

class mongoDbCofig
	:public Singleton<mongoDbCofig>
{
public:
	mongoDbCofig();
    ~mongoDbCofig() = default;

	bool LoadCfgFromFile();

	cfgMongo& getOfflineMsg();
	cfgMongo& getChatRecord();
	cfgMongo& getGroupRecord();
private:
	//离线消息数据库
	cfgMongo cfg_offline_msg;
	//聊天记录数据库
	cfgMongo cfg_user_record;
	cfgMongo cfg_group_record;
};
#endif //ALL_DBSRVCONFIGMGR_H
