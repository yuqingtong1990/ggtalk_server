//
// Created by yqt on 18-11-6.
//
#include <iostream>
#include <zconf.h>
#include "MessageDBControl.h"
#include "json/json.h"

int main(int agrc,char* agrcs[]){

    MessageDBControl::getSingletonPtr()->Init();

    Json::Value msg;
    msg["sendid"] = 11111;
    msg["recvid"] = 22222;
    msg["type"] = 1;
    msg["session"] = 1;
    msg["ver"] = 1;
    msg["uuid"] = 1111111111111111;
    msg["time"] = time(nullptr);
    Json::Value data;
    data["content"] = "空山星雨后";
    msg["data"] = data;

    MessageDBControl::getSingletonPtr()->GetUserOffLineMsg(1111111,false);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        MongoAsynMgr::getSingletonPtr()->onTimer();
    }
    return 0;
}

/*
 *
 *
 * #include <bsoncxx/json.hpp>
#include "mongocxx/client.hpp"
#include "mongocxx/instance.hpp"
#include <mongocxx/stdx.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

 *  using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::open_document;



    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    mongocxx::database db = client["chat_record"];

    std::vector<bsoncxx::document::value> documents;
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
    << "uid" << "20"
    << "remark" << ""
    << "username" << "yqt"
    << "encry" << ""
    << "_mark" << static_cast<int32_t>(0)
    << "_result" << ""
    << "ip_info" << "192.168.32.119"
    << bsoncxx::builder::stream::finalize;
   try{
    auto result = db["u_1"].insert_one(doc_value.view());
    if (!result) {
        std::cout<<"Unacknowledged write. No id available. doc_value_cr=\r\n"<<bsoncxx::to_json(doc_value.view()).c_str() <<std::endl;
    } else
    {
        std::cout<<"success"<<std::endl;
    }

    if (result->inserted_id().type() == bsoncxx::type::k_oid) {
        bsoncxx::oid id = result->inserted_id().get_oid().value;
        std::string id_str = id.to_string();
    }else{
        std::cout<<"Inserted id was not an OID type. doc_value_cr=%s \r\n"<<bsoncxx::to_json(doc_value.view()).c_str()<<std::endl;
    }

}catch (const std::exception & e){
    //Logger.Log(ERROR, "CGLConnMgr::Handle_PlayerAddGolds:%s", e.what());
    //Logger.Log(ERROR, "CGLConnMgr::Handle_PlayerAddGolds failed doc_value=%s \r\n", bsoncxx::to_json(doc_value.view()).c_str());
       std::cout<<"IGLConnMgr::Handle_PlayerAddGolds failed doc_value= \r\n"<<bsoncxx::to_json(doc_value.view()).c_str()<<std::endl;
}*/
