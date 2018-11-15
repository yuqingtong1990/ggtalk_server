//
// Created by yqt on 18-11-15.
//

#include "StringUtil.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

using namespace std;

std::string& ReplaceString( std::string &s, const std::string &OldPattern, const std::string &NewPattern )
{
	if (OldPattern == NewPattern)
		return s;
	string::size_type i;
	while (true)
	{
		i = s.find(OldPattern);
		if (i == string::npos)
			break;
		else
			s = s.replace(i, OldPattern.length(), NewPattern);
	}
	return s;
}

std::wstring& ReplaceStringW( std::wstring &s, const std::wstring &OldPattern, const std::wstring &NewPattern )
{
	if (OldPattern == NewPattern)
		return s;
	wstring::size_type i;
	while (true)
	{
		i = s.find(OldPattern);
		if (i == wstring::npos)
			break;
		else
			s = s.replace(i, OldPattern.length(), NewPattern);
	}
	return s;
}

struct CharCmpI
{
	bool operator()(char a, char b)
	{
		return toupper(a) == toupper(b);
	}
};

struct CharCmpIW
{
	bool operator()(wchar_t a, wchar_t b)
	{
		return toupper(a) == toupper(b);
	}
};

string& ReplaceStringI( string &s, const string &OldPattern, const string &NewPattern )
{
	if (strcmp(OldPattern.c_str(), NewPattern.c_str()) ==  0)
		return s;
	string::iterator it, itend;
	do{
		it = search(s.begin(), s.end(), OldPattern.begin(), OldPattern.end(), CharCmpI());
		if (it != s.end())
		{
			itend = it + OldPattern.length();
			s = s.replace(it, itend, NewPattern);
		}
		else
			break;
	} while (true);
	return s;
}

wstring& ReplaceStringIW( wstring &s, const wstring &OldPattern, const wstring &NewPattern )
{
	if (wcscmp(OldPattern.c_str(), NewPattern.c_str())  == 0)
		return s;
	wstring::iterator it, itend;
	do{
		it = search(s.begin(), s.end(), OldPattern.begin(), OldPattern.end(), CharCmpIW());
		if (it != s.end())
		{
			itend = it + OldPattern.length();
			s = s.replace(it, itend, NewPattern);
		}
		else
			break;
	} while (true);
	return s;
}

void SplitString( const string &AString, const string &ASplitStr, vector<string> &AStrings )
{
	const char *sdata = AString.c_str();
	const char *ssplit = ASplitStr.c_str();
	int i = 0, j = 0, iSizeA = AString.length(), iSizeB = ASplitStr.length();
	AStrings.clear();
	while (i < iSizeA)
	{
		if (i + iSizeB > iSizeA)
		{
			AStrings.push_back(AString.substr(j));
			j = i;
			break;
		}
		if (memcmp(&(sdata[i]), ssplit, iSizeB) == 0)
		{
			//if (j < i)
			AStrings.push_back(AString.substr(j, i - j));
			j = i + iSizeB;
			i += iSizeB;
		}
		else
			++i;
	}
	if (j < i)
		AStrings.push_back(AString.substr(j));
	return;
}

void SplitString( const string &AString, const string &ASplitStr, string &ALeft, string &ARight )
{
	vector<string> vs;
	SplitString(AString, ASplitStr, vs);
	ALeft = vs[0];
	ARight = "";
	if (vs.size() > 1)
	{
		for (size_t i = 1; i != vs.size(); ++i)
		{
			if (ARight.empty())
				ARight = vs[i];
			else
				ARight = ARight + ASplitStr + vs[i];
		}
	}
	else
		ARight = "";
	return;
}

void SplitStringW( const wstring &AString, const wstring &ASplitStr, vector<wstring> &dest )
{
	wstring::size_type pos = wstring::npos;
	wstring::size_type offset = 0;
	dest.clear();
	while (1)
	{
		pos = AString.find(ASplitStr.c_str(), offset);
		if (wstring::npos == pos)
			break;
		dest.push_back(AString.substr(offset, pos - offset));
		offset = ASplitStr.length() + pos;

	}
	if (offset != AString.length())
		dest.push_back(AString.substr(offset));
}

void SplitStringW( const wstring &AString, const wstring &ASplitStr, wstring &ALeft, wstring &ARight )
{
	vector<wstring> vs;
	SplitStringW(AString, ASplitStr, vs);
	ALeft = vs[0];
	if (vs.size() > 1)
	{
		for (size_t i = 1; i != vs.size(); ++i)
		{
			if (ARight.empty())
				ARight = vs[i];
			else
				ARight = ARight + ASplitStr + vs[i];
		}
	}
	else
		ARight = L"";
	return;
}



std::string ConcatString(std::vector<std::string> vecStr)
{
	if (0 == vecStr.size())
		return "";
	string strRet;
	for (unsigned int i = 0; i<vecStr.size(); i++)
	{
		strRet += vecStr[i];
	}
	return strRet;
}

std::wstring ConcatStringW(std::vector<std::wstring> vecStr)
{
	if (0 == vecStr.size())
		return L"";
	wstring strRet;
	for (unsigned int i = 0; i<vecStr.size(); i++)
	{
		strRet += vecStr[i];
	}
	return strRet;
}

std::string IntList2String(const std::vector<int> &AStringList, const std::string &AConnectStr)
{
	std::string ret;
	for (vector<int>::const_iterator it = AStringList.begin(); it != AStringList.end(); ++it)
	{
		std::string str = Int2Str(*it);
		if (ret.empty())
			ret = str;
		else
			ret = ret + AConnectStr + str;
	}
	return ret;
}

std::wstring IntList2StringW(const std::vector<int> &AStringList, const std::wstring &AConnectStr)
{
	wstring ret;
	for (vector<int>::const_iterator it = AStringList.begin(); it != AStringList.end(); ++it)
	{
		std::wstring wstr = Int2StrW(*it);
		if (ret.empty())
			ret = wstr;
		else
			ret = ret + AConnectStr + wstr;
	}
	return ret;
}

std::string StringList2String( const vector<string> &AStringList, const string &AConnectStr )
{
	string ret;
	for (vector<string>::const_iterator it = AStringList.begin(); it != AStringList.end(); ++it)
	{
		if (ret.empty())
			ret = *it;
		else
			ret = ret + AConnectStr + *it;
	}
	return ret;
}

std::string StringList2String( const list<string> &AStringList, const string &AConnectStr )
{
	string ret;
	for (list<string>::const_iterator it = AStringList.begin(); it != AStringList.end(); ++it)
	{
		if (ret.empty())
			ret = *it;
		else
			ret = ret + AConnectStr + *it;
	}
	return ret;
}

wstring StringList2StringW( const vector<wstring> &AStringList, const wstring &AConnectStr )
{
	wstring ret;
	for (vector<wstring>::const_iterator it = AStringList.begin(); it != AStringList.end(); ++it)
	{
		if (ret.empty())
			ret = *it;
		else
			ret = ret + AConnectStr + *it;
	}
	return ret;
}

wstring StringList2StringW( const list<wstring> &AStringList, const wstring &AConnectStr )
{
	wstring ret;
	for (list<wstring>::const_iterator it = AStringList.begin(); it != AStringList.end(); ++it)
	{
		if (ret.empty())
			ret = *it;
		else
			ret = ret + AConnectStr + *it;
	}
	return ret;
}

static char char2print(char a)
	{
		if ((a >= 32) && (a <= 126))return a;
		return '.';
	}

static std::string bin2print(const std::string &in, int size)
{
    const char *buf = in.c_str();
    std::string out("");
    std::string inp("");
    for (unsigned int i = 0; i < (unsigned)size; i++)
    {
        unsigned char tmp = i < in.length() ? buf[i] : ' ';
        inp += char2print(tmp);
        out += i < in.length() ? int2hex(tmp >> 4) : ' ';
        out += i < in.length() ? int2hex(tmp & 0x0f) : ' ';
        out += ' ';
        if ((i % 4) == 3) out += ' ';
    }
    out += inp + "\r\n";
    return out;
}

std::string bin2hex(const std::string &in, int size) {
    const char *buf = in.c_str();
    std::string out("");
    int i = 0, j = 0;
    for (i; (unsigned)i < in.length(); i++)
    {
        char tmp = buf[i];
        if (!size)
        {
            out += int2hex((tmp >> 4) & 0x0f);
            out += int2hex(tmp & 0x0f);
        }
        else
        {
            out += bin2print(in.substr(i, size), size);
            i += size - 1;
        }
    }

    return out;
}

std::string hex2bin(const std::string &in) {
    const char *buf = in.c_str();
    std::string out("");
    int i = 0;
    for (i; (unsigned)i < in.length(); i += 2)
    {
        if (((buf[i] >= '0' && buf[i] <= '9') || (buf[i] >= 'a' && buf[i] <= 'f'))
            && ((buf[i + 1] >= '0' && buf[i + 1] <= '9') || (buf[i + 1] >= 'a' && buf[i + 1] <= 'f'))) {
            char tmp = hex2int(buf[i]);
            char tmp1 = hex2int(buf[i + 1]);
            tmp = (tmp << 4) + tmp1;
            out += tmp;
        }
    }
    return out;
}

/*
 string 转换为 wstring
 */
std::wstring c2w(const char *pc)
{
	std::wstring val = L"";

	if (NULL == pc)
	{
		return val;
	}
	//size_t size_of_ch = strlen(pc)*sizeof(char);
	//size_t size_of_wc = get_wchar_size(pc);
	size_t size_of_wc;
	size_t destlen = mbstowcs(0, pc, 0);
	if (destlen == (size_t) (-1))
	{
		return val;
	}
	size_of_wc = destlen + 1;
	wchar_t * pw = new wchar_t[size_of_wc];
	mbstowcs(pw, pc, size_of_wc);
	val = pw;
	delete pw;
	return val;
}
/*
 wstring 转换为 string
 */
std::string w2c(const wchar_t * pw)
{
	std::string val = "";
	if (!pw)
	{
		return val;
	}
	size_t size = wcslen(pw) * sizeof(wchar_t);
	char *pc = NULL;
	if (!(pc = (char*) malloc(size)))
	{
		return val;
	}
	size_t destlen = wcstombs(pc, pw, size);
	/*转换不为空时，返回值为-1。如果为空，返回值0*/
	if (destlen == (size_t) (0))
	{
		return val;
	}
	val = pc;
	delete pc;
	return val;
}

string trim(const string &str)
{
	if (str == "")
		return str;
	int i = -1, j = -1;
	size_t k;
	for (k = 0; k < str.length(); ++k)
	{
		if (unsigned(str[k]) > unsigned(' '))
		{
			i = k;
			break;
		}
	}
	for (k = str.length() - 1; k >= 0; --k)
	{
		if (unsigned(str[k]) > unsigned(' '))
		{
			j = k;
			break;
		}
	}
	i = (i == -1 ? 0 : i);
	j = (j == -1 ? str.length() - 1 : j);
	return str.substr(i, j - i + 1);
}

std::wstring trimW(const wstring &str)
{
	if (str.empty())
		return str;
	int i = -1, j = -1;
	size_t k;
	for (k = 0; k < str.length(); ++k)
	{
		if (unsigned(str[k]) > unsigned(L' '))
		{
			i = k;
			break;
		}
	}
	for (k = str.length() - 1; k >= 0; --k)
	{
		if (unsigned(str[k]) > unsigned(L' '))
		{
			j = k;
			break;
		}
	}
	i = (i == -1 ? 0 : i);
	j = (j == -1 ? str.length() - 1 : j);
	return str.substr(i, j - i + 1);
}

wstring string2wstring(const string &s)
{
	return c2w(s.c_str());
}
string wstring2string(const wstring &s)
{
	return w2c(s.c_str());
}

std::string Int2Str(long i) {
    char ret[20] = { 0 };
	sprintf(ret, "%ld", i);
	return ret;
}

std::wstring Int2StrW(long i) {
    return std::__cxx11::wstring();
}



