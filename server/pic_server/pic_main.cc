//
// Created by yqt on 18-11-6.
//
#include <iostream>
#include <fstream>
#include <base/UniqueId.h>
#include <base/fileHandle.h>
#include "pic_server.h"
#include "base/httpformdata.h"
using namespace std;

int main(int agrc,char* agrcs[]){

    /*MultipartParser parser("0ca1e7ece412414db9f4392580c1fa45");
    std::ifstream t("/tmp/1.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string contents(buffer.str());
    bool b = parser.ParseBody(contents);*/
     /*
    std::string str = CreateRandomStr();
    cout<<str<<endl;
    cout<<str.size()<<endl;
    */
    evpp::EventLoop loop;
    PicServer server;
    server.Start(&loop,9100);
    server.Loop();

    return 0;
}

