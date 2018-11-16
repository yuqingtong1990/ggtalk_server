//
// Created by yqt on 18-11-16.
//

#include "httpformdata.h"
#include <iostream>
#include "StringUtil.h"
#include "UniqueId.h"
using namespace std;

void
MultipartBuilder::_get_file_name_type(const std::string &file_path, std::string *filename, std::string *content_type) {
    if (filename == nullptr || content_type == nullptr)
             return;

        size_t last_spliter = file_path.find_last_of("/\\");
        *filename = file_path.substr(last_spliter + 1);
        size_t dot_pos = filename->find_last_of(".");
        if (dot_pos == std::string::npos)
        {
            *content_type = "application/octet-stream";
            return;
        }
        std::string ext = filename->substr(dot_pos + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext == "jpg" || ext == "jpeg")
        {
            *content_type = "image/jpeg";
            return;
        }
        else if (ext == "txt" || ext == "log")
        {
            *content_type = "text/plain";
            return;
        }
        else{
            *content_type = "application/octet-stream";
        }
}

const std::string &MultipartBuilder::GenBodyContent() {
    std::vector<std::future<std::string> > futures;
      body_content_.clear();
      for(auto &file:files_)
      {
        std::future<std::string> content_futures = std::async(std::launch::async, [&file]()
        {
          std::ifstream ifile(file.second, std::ios::binary | std::ios::ate);
          std::streamsize size = ifile.tellg();
          ifile.seekg(0, std::ios::beg);
          char *buff = new char[size];
          ifile.read(buff, size);
          ifile.close();
          std::string ret(buff, size);
          delete[] buff;
          return ret;
        });
        futures.push_back(std::move(content_futures));
      }

      for(auto &param:params_)
      {
        body_content_ += "\r\n--";
        body_content_ += boundary_;
        body_content_ += "\r\nContent-Disposition: form-data; name=\"";
        body_content_ += param.first;
        body_content_ += "\"\r\n\r\n";
        body_content_ += param.second;
      }

      for(size_t i = 0; i < files_.size(); ++i)
      {
        std::string filename;
        std::string content_type;
        std::string file_content = futures[i].get();
        _get_file_name_type(files_[i].second, &filename, &content_type);
        body_content_ += "\r\n--";
        body_content_ += boundary_;
        body_content_ += "\r\nContent-Disposition: form-data; name=\"";
        body_content_ += files_[i].first;
        body_content_ += "\"; filename=\"";
        body_content_ += filename;
        body_content_ += "\"\r\nContent-Type: ";
        body_content_ += content_type;
        body_content_ += "\r\n\r\n";
        body_content_ += file_content;
      }


    body_content_ += "\r\n--";
    body_content_ += boundary_;
    body_content_ += "--\r\n";
    return body_content_;
}

void MultipartBuilder::generateboundary() {
    boundary_ = CreateRandomStr(32);
}

/*

 Content-Type: multipart/form-data; boundary=${boundary}

--${boundary}  //开始符号
...
...

--${boundary}-- //结束符号

例如
--c419e92b2e194c25839f5b0d324382a4
Content-Disposition: form-data; name="images1"; filename="1.txt"

hello world
--c419e92b2e194c25839f5b0d324382a4
Content-Disposition: form-data; name="images2"; filename="1.txt"

hello world
--c419e92b2e194c25839f5b0d324382a4
Content-Disposition: form-data; name="images3"; filename="1.txt"

hello world
--c419e92b2e194c25839f5b0d324382a4
Content-Disposition: form-data; name="images4"; filename="1.txt"

hello world
--c419e92b2e194c25839f5b0d324382a4
Content-Disposition: form-data; name="images5"; filename="1.txt"

hello world
--c419e92b2e194c25839f5b0d324382a4--

 * */
std::string MultipartParser::getboundary(bool bEnd /*= false*/){
    std::string strRet;
    if (bEnd){
        strRet = "--"+boundary_+"--";
    } else{
        strRet = "--"+boundary_;
    }
    return strRet;
}

bool MultipartParser::ParseBody(const std::string &strbody) {

    mapdescData_.clear();
   if (boundary_.empty())
            return false;

        size_t nEnd = strbody.find(getboundary(true));
        if (nEnd == std::string::npos)
            return false;

        size_t nStart = strbody.find(getboundary());
        if (nStart == std::string::npos)
            return false;

        //std::cout<<"start pos is " << nStart <<" end pos is " << nEnd << std::endl;

        const char* sep1 = "\r\n\r\n";
        const char* sep2 = "\r\n";

        std::string Spl = getboundary()+sep2;
        std::string strData = strbody.substr(nStart+Spl.size(),nEnd-nStart-2-Spl.size());
        //std::cout<<strData<<std::endl;
        std::vector<string> vec;
        SplitString(strData,Spl,vec);
        for (auto iter:vec) {
            string desc;
            string data;
            SplitString(iter,sep1,desc,data);
            mapdescData_[desc] = data;
        }
        return true;
}

void MultipartParser::Print() {
    for (auto iter: mapdescData_) {
        std::cout<<"httpfrom desc:"<<iter.first<<endl;
        std::cout<<"httpfrom data:"<<iter.second<<endl;
    }
}

std::string MultipartParser::QneryFindFromDisposition(const std::string &content, const std::string &key) {
    static const std::string __s_nullptr;
    std::cout<<content<<endl;
    //std::cout<<key<<endl;
    size_t n = content.find(key);
     //std::cout<<n<<endl;
    if (n == std::string::npos){
        return  __s_nullptr;
    }
    std::cout<<content[n+key.size()+1]<<endl;
    std::cout<<content[n+key.size()+2]<<endl;

    if(content[n+key.size()] != '=' || content[n+key.size()+1] != '\"'){
        return  __s_nullptr;
    }

    size_t nstart = n+key.size()+2;
    size_t nEnd = content.find('\"',nstart);
    if (nEnd == std::string::npos){
        return  __s_nullptr;
    }
    return content.substr(nstart,nEnd-nstart);
}

bool ParseAskeyValue(const std::string &data, Http_keyValue &kv) {
    bool bSuccess = false;
    do{
        std::vector<string> vec;
        SplitString(data,"&",vec);

        if (vec.empty())
            break;
        for(auto iter:vec){

            std::string strright;
            std::string strleft;
            SplitString(iter,"=",strleft,strright);
            if (strleft.empty() || strleft.empty())
                break;
            kv[strleft] = strright;
        }
        bSuccess = true;
    }while (0);

    return bSuccess;
}
