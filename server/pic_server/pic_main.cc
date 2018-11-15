//
// Created by yqt on 18-11-6.
//
#include <iostream>
#include "pic_server.h"

int main(int agrc,char* agrcs[]){

    evpp::EventLoop loop;
    PicServer server;
    server.Start(&loop,9100);
    server.Loop();
    return 0;
}

