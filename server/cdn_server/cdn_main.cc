#include <iostream>
#include "cdn_server.h"

int main(int agrc,char* agrcs[]){

    evpp::EventLoop loop;
    CDNServer server;
    server.Start(&loop,"0.0.0.0",8100);
    server.Loop();
    getchar();
    return 0;
}

