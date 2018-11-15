//
// Created by yqt on 18-11-15.
//

#ifndef ALL_HTTP_FORMDATA_H
#define ALL_HTTP_FORMDATA_H

#include "evpp/http/context.h"

typedef std::map<std::string,std::string> Http_keyValue;

class Content_Extension
{
public:
    Content_Extension(evpp::http::ContextPtr ptr)
            :ctx_(ptr){}

    ~Content_Extension() = default;

    //这里只支持这4种
    enum httpcontenttype{
        /*
         * 不支持的格式
         * */
        nosupport = 0,

        /*
         * multipart/form-data
         * 将表单的数据处理为一条消息，以标签为单元，用分隔符(boundary)分开。既可以上传键值对，也可以上传文件。
         * 当上传的字段是文件时，会有Content-Type来表名文件类型；content-disposition，用来说明字段的一些信息
         * */
        http_formdata  = 1,


        /*
         *就是application/x-www-from-urlencoded,
         * 会将表单内的数据转换为键值对 key1=value1&key1=value2
         * */
        x_www_from_urlencoded,

        /*
         * content-type:application/json text/json
         * */
        json,


        /*
         *content-type:text/xml
         * */
        xml,

        /*
         * 可以上传任意格式的文本，可以上传text、json、xml、html等
         * content-type包含 text
         * */
        raw,

        /*
         * 相当于Content-Type:application/octet-stream,
         * 从字面意思得知，只可以上传二进制数据，通常用来上传文件，由于没有键值，所以，一次只能上传一个文件
         * */
        binary,
    };

    std::string getContentType();
    httpcontenttype convType(const std::string& strType);
    Http_keyValue ParseAskeyValue();

private:
    evpp::http::ContextPtr ctx_;
};



#endif //ALL_HTTP_FORMDATA_H
