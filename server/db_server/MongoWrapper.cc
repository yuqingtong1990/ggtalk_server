#include "MongoWrapper.h"

#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/exception/exception.hpp>

MongoWrapper::MongoWrapper(): _conn(nullptr) {

}

MongoWrapper::~MongoWrapper() {
    if (_conn)
    {
        delete _conn;
        _conn = nullptr;
    }
}

static mongocxx::instance* _instance = nullptr;

bool MongoWrapper::initMongodb() {
    if (!_instance)
        _instance = new(mongocxx::instance);
    return false;
}

void MongoWrapper::shutdown() {

}

MongoCode MongoWrapper::initConnection(const std::string &host_name, ReadMode read_mode, const std::string &replset,
                                       const std::string &db_name, const std::string &username,
                                       const std::string &password) {
    MongoCode e_code = E_SUCCESS;
    try {
        if (_conn)
        {
            delete _conn;
            _conn = nullptr;
        }
        _url = host_name;
        _read_mode = read_mode;
        _dbName = db_name;
        _username = username;
        _password = password;
        _replset = replset;

        if (replset.empty()) {
            mongocxx::v_noabi::uri url("mongodb://" + username + ":" + password + "@" + host_name + "/?authSource=" + db_name);
            _conn = new mongocxx::client(url);
        } else {
            mongocxx::v_noabi::uri url("mongodb://" + username + ":" + password + "@" + host_name + "/?replicaSet=" + replset);
            _conn = new mongocxx::client(url);
        }
        mongocxx::read_preference::deprecated_tag tag;
        switch (read_mode)
        {
        case k_primary:
            _conn->read_preference_deprecated(mongocxx::read_preference(mongocxx::read_preference::read_mode::k_primary,tag));
            break;
        case k_primary_preferred:
            _conn->read_preference_deprecated(mongocxx::read_preference(mongocxx::read_preference::read_mode::k_primary_preferred,tag));
            break;
        case k_secondary:
            _conn->read_preference_deprecated(mongocxx::read_preference(mongocxx::read_preference::read_mode::k_secondary,tag));
            break;
        case k_secondary_preferred:
            _conn->read_preference_deprecated(mongocxx::read_preference(mongocxx::read_preference::read_mode::k_secondary_preferred,tag));
            break;
        case k_nearest:
            _conn->read_preference_deprecated(mongocxx::read_preference(mongocxx::read_preference::read_mode::k_nearest,tag));
            break;
        default:
            break;
        }

        auto mode = _conn->read_preference().mode();
        _db = (*_conn)[db_name.c_str()];

    }catch (mongocxx::v_noabi::exception& ex){
        delete _conn;
        _conn = nullptr;
        e_code = E_CONNECTION_FAILS;
    }

    return e_code;
}

MongoCode MongoWrapper::resetConnect() {
    if (!_conn)
            return initConnection(_url, _read_mode, _replset, _dbName, _username, _password);
        return E_SUCCESS;
}

MongoCode MongoWrapper::count(std::int64_t &result, const std::string &ns, const std::string &query) {
    MongoCode error = ping();

    if (error != MongoCode::E_SUCCESS)
        return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::count options;
        //auto hint = bsoncxx::document::view_or_value{ bsoncxx::v_noabi::builder::basic::make_document(bsoncxx::v_noabi::builder::basic::kvp("_id", 1))};
        //CHECK_OPTIONAL_ARGUMENT(count, hint, hint);
        result = coll.count_deprecated({bsoncxx::v_noabi::from_json(query)}, options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
    }

    return error;
}

MongoCode
MongoWrapper::selectOne(bsoncxx::v_noabi::stdx::optional<bsoncxx::document::value> &result, const std::string &ns,
                        const std::string &query, const std::string &filter) {
    MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::find options;
        options.limit(1);
        if (!filter.empty())
        {
            options.projection(bsoncxx::v_noabi::from_json(filter));
        }
        result = coll.find_one(bsoncxx::v_noabi::from_json(query), options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.findOne(%s,%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), filter.empty() ? "null" : filter.c_str(), ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.findOne(%s,%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), filter.empty() ? "null" : filter.c_str(), ex.code().value(), ex.what());
    }
    return error;
}

MongoCode MongoWrapper::selectMany(MongoWrapper::autoCursor &result, const std::string &ns, const std::string &query,
                                   const std::string &filter, const std::string &sort, int64_t limit, int64_t skip) {
    MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::find options;
        options.limit(limit);
        options.skip(skip);
        if (!sort.empty()) {
            options.sort(bsoncxx::v_noabi::from_json(sort));
        }
        if (!filter.empty()) {
            options.projection(bsoncxx::v_noabi::from_json(filter));
        }
        result = autoCursor(new mongocxx::v_noabi::cursor(coll.find(bsoncxx::v_noabi::from_json(query), options)));
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.find(%s,%s).sort(%d).limit(%d).skip(%d)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), filter.empty() ? "null" : filter.c_str(), sort.empty() ? "null" : sort.c_str(), limit, skip, ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.find(%s,%s).sort(%d).limit(%d).skip(%d)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), filter.empty() ? "null" : filter.c_str(), sort.empty() ? "null" : sort.c_str(), limit, skip, ex.code().value(), ex.what());
    }
    return error;
}

MongoCode MongoWrapper::insertOne(const std::string &ns, const std::string &data) {
    MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::insert options;
        options.ordered(false);
        coll.insert_one(bsoncxx::v_noabi::from_json(data), options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.insertOne(%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), data.empty() ? "null" : data.c_str(), ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        auto errMsg = ex.what();
        //LOG_ERROR("\n\tdb.%s.insertOne(%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), data.empty() ? "null" : data.c_str(), ex.code().value(), ex.what());
    }
    return error;
}

MongoCode MongoWrapper::insertMany(const std::string &ns, const std::string &data) {
    MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        std::vector<bsoncxx::v_noabi::document::view> documents_to_insert{};
        auto json = bsoncxx::v_noabi::from_json(data);
        for (auto&& element : json.view())
        {
            documents_to_insert.push_back(element.get_document().value);
        }
        auto coll = _db[ns];
        mongocxx::v_noabi::options::insert options;
        options.ordered(false);
        coll.insert_many(documents_to_insert, options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.insert(%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), data.empty() ? "null" : data.c_str(), ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.insert(%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), data.empty() ? "null" : data.c_str(), ex.code().value(), ex.what());
    }
    return error;
}

MongoCode
MongoWrapper::updateOne(const std::string &ns, const std::string &query, const std::string &data, bool upsert) {
    MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::update options;
        options.upsert(upsert);
        coll.update_one(bsoncxx::v_noabi::from_json(query), bsoncxx::v_noabi::from_json(data), options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.updateOne(%s,%s,{upsert:%d})\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), data.empty() ? "null" : data.c_str(), upsert, ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.updateOne(%s,%s,{upsert:%d})\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), data.empty() ? "null" : data.c_str(), upsert, ex.code().value(), ex.what());
    }
    return error;
}

MongoCode
MongoWrapper::updateMany(const std::string &ns, const std::string &query, const std::string &data, bool upsert) {
    MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::update options;
        options.upsert(upsert);
        coll.update_many(bsoncxx::v_noabi::from_json(query), bsoncxx::v_noabi::from_json(data), options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.updateMany(%s,%s,{upsert:%d})\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), data.empty() ? "null" : data.c_str(), upsert, ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.%s.updateMany(%s,%s,{upsert:%d})\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), data.empty() ? "null" : data.c_str(), upsert, ex.code().value(), ex.what());
    }
    return error;
}

MongoCode MongoWrapper::deleteOne(const std::string &ns, const std::string &query) {
     MongoCode error = ping();
    if (error != MongoCode::E_SUCCESS) return error;
    try
    {
        auto coll = _db[ns];
        mongocxx::v_noabi::options::delete_options options;
        coll.delete_one(bsoncxx::v_noabi::from_json(query), options);
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.deleteOne(%s,%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("\n\tdb.deleteOne(%s,%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), ex.code().value(), ex.what());
    }
    return error;
}

MongoCode MongoWrapper::deleteMany(const std::string &ns, const std::string &query) {
    MongoCode error = ping();
        if (error != MongoCode::E_SUCCESS) return error;
        try
        {
            auto coll = _db[ns];
            mongocxx::v_noabi::options::delete_options options;
            coll.delete_many(bsoncxx::v_noabi::from_json(query), options);
        }
        catch (bsoncxx::exception &ex)
        {
            error = E_DB_EXCEPTION;
           // LOG_ERROR("\n\tdb.deleteMany(%s,%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), ex.code().value(), ex.what());
        }
        catch (mongocxx::v_noabi::exception &ex)
        {
            error = E_DB_EXCEPTION;
           // LOG_ERROR("\n\tdb.deleteMany(%s,%s)\n\tException Occur errcode:%d errmsg:%s", ns.empty() ? "null" : ns.c_str(), query.empty() ? "null" : query.c_str(), ex.code().value(), ex.what());
        }

        return error;
}

MongoCode MongoWrapper::drop(const std::string &ns) {
    MongoCode error = ping();
        if (error != MongoCode::E_SUCCESS) return error;
        try
        {
            auto coll = _db[ns];
            coll.drop();
        }
        catch (bsoncxx::exception &ex)
        {
            error = E_DB_EXCEPTION;
            //LOG_ERROR("\n\tdb.%s.drop()\n\tException Occur errcode:%d errormsg:%s", ns.empty() ? "null" : ns.c_str(), ex.code().value(), ex.what());
        }
        catch (mongocxx::v_noabi::exception &ex)
        {
            error = E_DB_EXCEPTION;
            //LOG_ERROR("\n\tdb.%s.drop()\n\tException Occur errcode:%d errormsg:%s", ns.empty() ? "null" : ns.c_str(), ex.code().value(), ex.what());
        }
        return error;
}

MongoCode MongoWrapper::ping() {
   if (_url.empty() || !_conn)
    {
        return E_CONNECTION_FAILS;
    }
    MongoCode error = E_SUCCESS;
    try
    {
        auto value = _db.run_command(bsoncxx::v_noabi::builder::basic::make_document(bsoncxx::v_noabi::builder::basic::kvp("ping", 1)));
        for (auto& it : value.view())
        {
            error = (it.key().compare("ok") == 0 && it.get_double() == 1.0f) ? E_SUCCESS : E_DB_EXCEPTION;
        }
    }
    catch (bsoncxx::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("MongoDB ping errcode:%d errormsg:%s", ex.code().value(), ex.what());
    }
    catch (mongocxx::v_noabi::exception &ex)
    {
        error = E_DB_EXCEPTION;
        //LOG_ERROR("MongoDB ping err code:%d errormsg:%s", ex.code().value(), ex.what());
    }
    return error;
}


