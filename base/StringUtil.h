//
// Created by yqt on 18-11-15.
//

#ifndef ALL_STRINGUTIL_H
#define ALL_STRINGUTIL_H

#include <string>
#include <vector>
#include <list>

#define SafeDelete(p)   if(p) { delete (p); (p) = NULL;}
#define SafeDelArry(p)  if(p) { delete []p; (p) = NULL;}
#define SafeFree(p)   if(p) { free (p); (p) = NULL;}
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))
#define SafeRelease(var) if(var) {var->Release(); var = NULL;}

#define int2hex(a) ((a)<10?(a)+'0':(a)+'a'-10)
#define hex2int(a) ((a)>='a'?(a)-'a'+10:(a)-'0')

std::wstring string2wstring(const std::string &s);
std::string wstring2string(const std::wstring &s);

std::string trim(const std::string &str);
std::wstring trimW(const std::wstring &str);

std::string Int2Str(long i);
std::wstring Int2StrW(long i);

std::string& ReplaceString(std::string &s, const std::string &OldPattern, const std::string &NewPattern);
std::wstring& ReplaceStringW(std::wstring &s, const std::wstring &OldPattern, const std::wstring &NewPattern);
std::string& ReplaceStringI(std::string &s, const std::string &OldPattern, const std::string &NewPattern);
std::wstring& ReplaceStringIW(std::wstring &s, const std::wstring &OldPattern, const std::wstring &NewPattern);
void SplitString(const std::string &AString, const std::string &ASplitStr, std::vector<std::string> &AStrings);
void SplitStringW(const std::wstring &AString, const std::wstring &ASplitStr, std::vector<std::wstring> &AStrings);
void SplitString(const std::string &AString, const std::string &ASplitStr, std::string &ALeft, std::string &ARight);
void SplitStringW(const std::wstring &AString, const std::wstring &ASplitStr, std::wstring &ALeft, std::wstring &ARight);

std::string IntList2String(const std::vector<int> &AStringList, const std::string &AConnectStr);
std::wstring IntList2StringW(const std::vector<int> &AStringList, const std::wstring &AConnectStr);
std::string StringList2String(const std::vector<std::string> &AStringList, const std::string &AConnectStr);
std::wstring StringList2StringW(const std::vector<std::wstring> &AStringList, const std::wstring &AConnectStr);
std::string StringList2String(const std::list<std::string> &AStringList, const std::string &AConnectStr);
std::wstring StringList2StringW(const std::list<std::wstring> &AStringList, const std::wstring &AConnectStr);

std::string ConcatString(std::vector<std::string> vecStr);
std::wstring ConcatStringW(std::vector<std::wstring> vecStr);

std::string UpperCase(const std::string &str);
std::wstring UpperCaseW(const std::wstring &str);
std::string LowerCase(const std::string &str);
std::wstring LowerCaseW(const std::wstring &str);

std::string bin2hex(const std::string &in, int size = 0);
std::string hex2bin(const std::string &in);

void CharToHex(unsigned char c, unsigned char* buf);
unsigned char HexToChar(const unsigned char* hex);
//buffer转16进制
std::string BufToHex(unsigned char *buf, unsigned long bufsize);
//16进制转buffer
void HexToBuf(const std::string &strhex,
/*out*/unsigned char **buf,
/*out*/unsigned long &bufsize);

std::string UrlDecode(const char* s);
std::string UrlEncode(const char *s);

std::string FormatString(const char* str, ...);
std::string Fmt(const char* str, ...);
std::wstring FormatStringW(const wchar_t* str, ...);
std::string FormatStringEx(size_t buf_size, const char* str, ...);
std::wstring FormatStringWEx(size_t buf_size, const wchar_t* str, ...);


std::string ExtractFilePath(const std::string &filestr, bool IncludeBackslash);
std::string ExtractFileName(const std::string &filestr);
std::string IncludeTrailingPathDelimiter(const std::string &path);
std::string ExcludeTrailingPathDelimiter(const std::string &path);
std::string ExtractExtensions(const std::string &filestr, bool IncludePoint = true);

#endif //ALL_STRINGUTIL_H
