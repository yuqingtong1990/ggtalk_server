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

std::string bin2hex(const std::string &in, int size = 0);
std::string hex2bin(const std::string &in);

#endif //ALL_STRINGUTIL_H
