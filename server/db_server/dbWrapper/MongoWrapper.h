//
// Created by yqt on 18-11-8.
//

#ifndef ALL_MONGOWRAPPER_H
#define ALL_MONGOWRAPPER_H

#include <string>
#include <memory>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/builder/basic/document.hpp"
#include "bsoncxx/json.hpp"

#include "mongocxx/client.hpp"
#include "mongocxx/instance.hpp"

#include "MongoDefine.h"


/*
 * create by yuqingtong
 * date：2018.11.7
 * base by https://github.com/hlxxiang/mongopp
 * Adaptive new mongodb driver
 * c driver 1.13.0
 * cxx driver r3.4.0
*/

struct MongoDbOpt{

    MongoDbOpt(int option, const std::string &table, const std::string& query, const std::string& date)
            :_upsert(false), _multi(false), _raw(false)
        ,_option(option),_table(table),_query(query),_data(date)
    {
    }

    int _option;
    std::string _table;
    std::string _query;
    std::string _data;
    bool _upsert;
    bool _multi;
    bool _raw;

};

typedef std::shared_ptr<MongoDbOpt> MongoDbOptPtr;

class MongoWrapper
{
     public:
        using BSONValueList = std::vector<bsoncxx::document::value>;
        using autoCursor = std::shared_ptr<mongocxx::v_noabi::cursor>;

        MongoWrapper();
        ~MongoWrapper();

        static bool initMongodb();
        static void shutdown();
        MongoCode initConnection(const std::string &host_name, const std::string &db_name);
        MongoCode initConnection(const std::string &host_name, ReadMode read_mode, const std::string& replset, const std::string &db_name, const std::string &username, const std::string &password);

        MongoCode count(std::int64_t &result, const std::string &ns, const std::string &query);
        MongoCode selectOne(bsoncxx::v_noabi::stdx::optional<bsoncxx::document::value> &result, const std::string &ns, const std::string &query, const std::string &filter);
        MongoCode selectMany(autoCursor &result, const std::string &ns, const std::string &query, const std::string &filter, const std::string& sort, int64_t limit = 0, int64_t skip = 0);
        MongoCode insertOne(const std::string &ns, const std::string &data);
        MongoCode insertMany(const std::string &ns, const std::string &data);
        MongoCode updateOne(const std::string &ns, const std::string& query, const std::string& data, bool upsert = true);
        MongoCode updateMany(const std::string &ns, const std::string& query, const std::string& data, bool upsert = true);
        MongoCode deleteOne(const std::string &ns, const std::string& query);
        MongoCode deleteMany(const std::string &ns, const std::string& query);
        MongoCode drop(const std::string &ns);
    private:

        MongoCode ping();

        ReadMode        _read_mode;
        std::string     _url;
        std::uint16_t   _port;
        std::string     _replset;
        std::string     _dbName;
        std::string     _username;
        std::string     _password;
        mongocxx::client* _conn = nullptr;
        mongocxx::database _db;
};

using MongoWrapperPtr = std::shared_ptr<MongoWrapper>;


#endif //ALL_MONGOWRAPPER_H
