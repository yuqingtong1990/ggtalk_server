//
// Created by yqt on 18-11-15.
//

#include "http_formdata.h"

std::string Content_Extension::getContentType() {
    if (ctx_){
        return ctx_->FindRequestHeader("Content-Type");
    }
    return "";
}

Content_Extension::httpcontenttype Content_Extension::convType(const std::string &strType) {

    httpcontenttype type;
    if (strType.compare == "multipart/form-data"){
        type = http_formdata;

    }else if (strType == "application/x-www-from-urlencoded"){
        type = x_www_from_urlencoded;
    }
    else if (strType == "application/json"){
        type = json;
    }
    else if (strType == "text/json"){
        type = json;
    }
    else if (strType == "application/xml"){
        type = xml;
    }
    else if (strType == "text/xml"){
        type = xml;
    }
    else if (strType == "application/octet-stream"){
        type = binary;
    }
    else {
        if (strType.find("text") != std::string::npos){
            type = raw;
        } else{
            type = nosupport;
        }
    }
    return nosupport;
}

Http_keyValue Content_Extension::ParseAskeyValue() {
    return Http_keyValue();
}
