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


#endif //ALL_DBSRVCONFIGMGR_H
