//
// Created by yqt on 18-11-16.
//

#ifndef ALL_HTTPFORMDATA_H
#define ALL_HTTPFORMDATA_H

#include <string>
#include <future>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>


/*
 * multipart/form-data格式的数据
 * */
class MultipartBuilder
{
public:
  MultipartBuilder(){};
  inline const std::string &body_content() { return body_content_; }
  inline const std::string &boundary() { return boundary_; }
  inline void AddParameter(const std::string &name, const std::string &value) {
    params_.push_back(std::move(std::pair<std::string, std::string>(name, value)));
  }
  inline void AddFile(const std::string &name, const std::string &value) {
    files_.push_back(std::move(std::pair<std::string, std::string>(name, value)));
  }
  const std::string &GenBodyContent();

private:
  void generateboundary();
  void _get_file_name_type(const std::string &file_path, std::string *filename, std::string *content_type);
private:
  std::string boundary_;
  std::string body_content_;
  std::vector<std::pair<std::string, std::string> > params_;
  std::vector<std::pair<std::string, std::string> > files_;
};

class MultipartParser{
public:
    explicit MultipartParser(const char* pboundary) :boundary_(pboundary) {}

    inline std::string setboundary(const std::string& boundary){ boundary_ = boundary; }

    std::string getboundary(bool bEnd = false);

    bool ParseBody(const std::string& strbody);
    void Print();
    std::map<std::string,std::string>& getform(){
        return mapdescData_;
    };

    static std::string QneryFindFromDisposition(const std::string& content,const std::string& key);
private:
    std::string boundary_;
    std::map<std::string,std::string> mapdescData_;
};


/*
 * x-www-form-urlencoded
 *
 * */
typedef std::map<std::string,std::string> Http_keyValue;
bool ParseAskeyValue(const std::string& data,Http_keyValue& kv);


#endif //ALL_HTTPFORMDATA_H
