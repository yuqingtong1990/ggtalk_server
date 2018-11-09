//
// Created by yqt on 18-11-9.
//

#include "DbSrvConfigMgr.h"

void mysqlDbConfig::Set(const std::string &host, const std::string &user, const std::string &password,
                        const std::string &dbname) {
    host_ = host;
    user_ = user;
    password_ = password;
    dbname_ = dbname;
}

bool mysqlDbConfig::LoadCfgFromFile() {
    return false;
}

std::string &mysqlDbConfig::host() {
    return host_;
}

std::string &mysqlDbConfig::user() {
    return user_;
}

std::string &mysqlDbConfig::password() {
    return password_;
}

std::string &mysqlDbConfig::dbname() {
    return dbname_;
}
