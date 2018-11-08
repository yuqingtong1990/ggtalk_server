//
// Created by yqt on 18-11-8.
//

#ifndef ALL_MONGOASYNMGR_H
#define ALL_MONGOASYNMGR_H

#include <thread>
#include "base/Singleton.h"
#include "base/fast_queue.h"
#include "MongoAysnOper.h"


using MongOperQueue = FastQueue<std::pair<std::string, MongoAsynOperPtr>>;

class MongoAsynMgr
: public Singleton<MongoAsynMgr>
{
    public:
        using MongoWrapperMap = std::unordered_map<std::string, MongoWrapperPtr>;
        using MongoWrapperMapPair = std::pair<std::string, MongoWrapperPtr>;

        MongoAsynMgr();
        ~MongoAsynMgr();

        bool addConnect(const std::string& dbKey, ReadMode read_mode, const std::string &host_name, const std::string& replset, const std::string &db_name, const std::string &username, const std::string &password);
        void delConnect(const std::string& dbKey);
        MongoWrapperPtr findMongoWrapper(const std::string& dbKey);
        bool start();
        void run();
        void stop();
        void runOper(const std::string& dbKey, MongoAsynOperPtr oper);
        void execute();
        void onTimer();
    private:
        MongoWrapperMap _warpper_map;

        MongOperQueue _operQueue;
        MongOperQueue _resultQueue;

        bool _loop;
        int _sleep;
        std::thread _thread;
};


#endif //ALL_MONGOASYNMGR_H
