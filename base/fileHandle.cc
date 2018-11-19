//
// Created by yqt on 18-11-19.
//

#include <zconf.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include "fileHandle.h"
#include "StringUtil.h"

using namespace std;

#ifdef __x86_64__
#define POINTER long long
#else
#define POINTER	long
#endif

inline void IncPtr(void **p, POINTER inc_bytes)
{
	*p = (void*) ((POINTER) *p + inc_bytes);
}

bool FileExists(const char *fn) {
    return 0 == access(fn, R_OK);
}

bool IsDirectory(const char *fn) {
    struct stat fs;
	if (-1 == stat(fn, &fs))
		return false;
	return S_ISDIR(fs.st_mode);
}

long GetFileSize(const char *fn) {
    struct stat fs;
	if (-1 == stat(fn, &fs))
		return -1;
	return (long) fs.st_size;
}

bool DirectoryExists(const char *fn) {
    return FileExists(fn) && IsDirectory(fn);
}

bool ForceDirectory(const std::string &fn) {
    	bool ret = true;
	if (fn.empty())
		return false;
	string sdir = ExcludeTrailingPathDelimiter(fn);
	if (sdir.length() < 3 || DirectoryExists(sdir.c_str())
			|| (ExtractFilePath(sdir, true) == sdir))
		return ret;
	return ForceDirectory(ExtractFilePath(sdir, true))
			&& 0
					== mkdir(sdir.c_str(),
							S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP
									| S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
}

bool LoadBufferFromFile(const std::string &file, void **buf, size_t &bufsize) {
   if (!FileExists(file.c_str()))
		return false;
	if (IsDirectory(file.c_str()))
		return false;
	long fsize = GetFileSize(file.c_str());
	if (-1 == fsize)
		return false;
	int fd = open(file.c_str(), O_RDONLY);
	if (-1 == fd)
		return false;
	*buf = malloc(fsize);
	bufsize = 0;
	void *p = *buf;
	long iretain = fsize;
	long irded = 0;
	while (iretain > 0)
	{
		irded = read(fd, p, iretain);
		if (-1 == irded)
		{
			free(*buf);
			*buf = nullptr;
			close(fd);
			return false;
		}
		iretain -= irded;
		IncPtr(&p, irded);
	}
	close(fd);
	bufsize = fsize;
	return true;
}

std::string LoadStringFromFile(const std::string &file) {
    std::ifstream t(file);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string contents(buffer.str());
}

bool SaveBufferToFile(const std::string &file, const void *buf, size_t bufsize) {
    if (file.empty())
		return false;
	if (nullptr == buf || 0 == bufsize)
		return false;
	int fd = open(file.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (-1 == fd)
		return false;
	long iretain = bufsize;
	long iw = 0;
	void *p = (void*)buf;
	while (iretain > 0)
	{
		iw = write(fd, p, static_cast<size_t>(iretain));
		if (-1 == iw)
		{
			close(fd);
			return false;
		}
		iretain -= iw;
		IncPtr(&p, iw);
	}
	close(fd);
	return true;
}

bool AppendBuf2File(const std::string& file, const void *buf, size_t bufsize) {
    if (file.empty())
		return false;
	if (nullptr == buf || 0 == bufsize)
		return false;
	int fd = open(file.c_str(), O_WRONLY | O_APPEND);
	if (-1 == fd)
		return false;
	long iretain = bufsize;
	long iw = 0;
	void *p = (void*) buf;
	while (iretain > 0)
	{
		iw = write(fd, p, static_cast<size_t>(iretain));
		if (-1 == iw)
		{
			close(fd);
			return false;
		}
		iretain -= iw;
		IncPtr(&p, iw);
	}
	close(fd);
	return true;
}

bool SaveBufferToFile(const std::string &path, const std::string &buf) {
     fstream f(path, ios::out | ios::trunc | ios::binary);
        f << buf;//写入数据
        f.close();
}

bool AppendBuf2File(const std::string &path, const std::string &buf) {
     fstream f(path, ios::out | ios::app | ios::binary);
     f << buf;//写入数据
     f.close();
}

std::string getHomePath() {
    char *path = getenv("HOME");
    return string(path);
}
