//
// Created by yqt on 18-11-6.
//
#include <iostream>
#include <bsoncxx/json.hpp>
#include "mongocxx/client.hpp"
#include "mongocxx/instance.hpp"

int main(int agrc,char* agrcs[]){
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);
    mongocxx::database db = client["chatrecord"];
    std::cout<<"connected ok"<<std::endl;
    return 0;
}

