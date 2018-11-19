//
// Created by yqt on 18-11-19.
//

/*
 * File operations class, currently only support Linux late will support Windows
 * */

#ifndef ALL_FILEHANDLE_H
#define ALL_FILEHANDLE_H

#include <string>

//under Linux folder is file but under Windows is not
bool FileExists(const char *fn);
bool IsDirectory(const char *fn);
long GetFileSize(const char *fn);
bool DirectoryExists(const char *fn);
bool ForceDirectory(const std::string &fn);
bool LoadBufferFromFile(const std::string &file,/*outer*/void **buf,/*outer*/size_t &bufsize);
bool SaveBufferToFile(const std::string &file, const void *buf, size_t bufsize);
bool AppendBuf2File(const std::string& file, const void *buf, size_t bufsize);


//C++ styles
std::string LoadStringFromFile(const std::string &file);
bool SaveBufferToFile(const std::string &path, const std::string& buf);
bool AppendBuf2File(const std::string& path, const std::string& buf);


//path
std::string getHomePath();
#endif //ALL_FILEHANDLE_H
