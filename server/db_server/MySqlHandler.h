//
// Created by yqt on 18-11-22.
//

#ifndef ALL_MYSQLHANDLER_H
#define ALL_MYSQLHANDLER_H

#include <string>
#include "dbWrapper/dbmysqltaskpool.h"
#include "ImPdu.h"

class DBLoginTask
	:public ITaskDB
{
public:
	DBLoginTask() = default;
	~DBLoginTask() = default;

	virtual void Process(std::shared_ptr<CDatabaseMysql> poConn);
private:
	bool isCellphone(const std::string& ss);
	bool isEmail(const std::string& ss);
};
#endif //ALL_MYSQLHANDLER_H
