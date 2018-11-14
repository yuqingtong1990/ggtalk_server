#pragma once

#include <string>
#include <map>
#include <vector>
#include <assert.h>
#include <algorithm>
#ifndef WIN32
#include <cstdint>
#else
#include <stdint.h>
#endif
#include <mysql/mysql.h>
#include <mysql/errmsg.h>

#define MAX_QUERY_LEN   1024


using namespace std;

inline void ToLowerString(string& str)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

class Field
{
    public:

        enum DataTypes
        {
            DB_TYPE_UNKNOWN = 0x00,
            DB_TYPE_STRING  = 0x01,
            DB_TYPE_INTEGER = 0x02,
            DB_TYPE_FLOAT   = 0x03,
            DB_TYPE_BOOL    = 0x04
        };

        Field();
        Field(Field &f);
        Field(const char *value, enum DataTypes type);

        ~Field();

        enum DataTypes GetType() const { return mType; }

        const string GetString() const { return  m_strValue; }
        std::string GetCppString() const
        {
            return  m_strValue;                    // std::string s = 0 have undefine result in C++
        }
        float GetFloat() const { return static_cast<float>(atof( m_strValue.c_str())); }
        bool GetBool() const { return  atoi(m_strValue.c_str()) > 0; }
        int32_t GetInt32() const { return  static_cast<int32_t>(atol(m_strValue.c_str())); }
        uint8_t GetUInt8() const { return  static_cast<uint8_t>(atol(m_strValue.c_str())); }
        uint16_t GetUInt16() const { return  static_cast<uint16_t>(atol(m_strValue.c_str())); }
        int16_t GetInt16() const { return  static_cast<int16_t>(atol(m_strValue.c_str())); }
        uint32_t GetUInt32() const { return  static_cast<uint32_t>(atol(m_strValue.c_str())); }
        uint64_t GetUInt64() const
        {
            uint64_t value = 0;
            value = atoll(m_strValue.c_str());
            return value;
        }

        void SetType(enum DataTypes type) { mType = type; }

        void SetValue(const char *value, size_t uLen);
        void SetName(const string& strName)
        {
            m_strFieldName = strName;
            ToLowerString(m_strFieldName);
        }
        const string& GetName()
        {
            return m_strFieldName;
        }

        bool IsNULL()
        {
            return m_bNULL;
        }

        template<typename T>
        void ConvertValue(T& value);

    private:
        string m_strValue;
        string m_strFieldName;
        enum DataTypes mType;

    public:
        bool m_bNULL;
};

class QueryResult
{
    public:
        typedef std::map<uint32_t, std::string> FieldNames;

        QueryResult(MYSQL_RES* result, uint64_t rowCount, uint32_t fieldCount, uint32_t mError);
        virtual ~QueryResult();

        virtual bool NextRow();

        uint32_t GetField_idx(const std::string& name) const
        {
            for(FieldNames::const_iterator iter = GetFieldNames().begin(); iter != GetFieldNames().end(); ++iter)
            {
                if(iter->second == name)
                    return iter->first;
            }
            assert(false && "unknown field name");
            return uint32_t(-1);
        }

        Field *Fetch() const { return mCurrentRow; }

        const Field & operator [] (int index) const
        {
            return mCurrentRow[index];
        }

        const Field & operator [] (const std::string &name) const
        {
            return mCurrentRow[GetField_idx(name)];
        }

        uint32_t GetError() const { return mError; }
		uint32_t GetFieldCount() const { return mFieldCount; }
        uint64_t GetRowCount() const { return mRowCount; }
        FieldNames const& GetFieldNames() const {return mFieldNames; }

        vector<string> const& GetNames() const {return m_vtFieldNames;}

    private:
        enum Field::DataTypes ConvertNativeType(enum_field_types mysqlType) const;
	public:
        void EndQuery();

    protected:
        Field *             mCurrentRow;
        uint32_t            mFieldCount;
        uint64_t            mRowCount;
        FieldNames          mFieldNames;
        std::vector<string> m_vtFieldNames;
		uint32_t			mError;
		MYSQL_RES*          mResult;
};


class CDatabaseMysql
{
public:
	struct DatabaseInfo
	{
		string strHost;
		string strUser;
		string strPwd;
		string strDBName;
	};
public:
	CDatabaseMysql(void);
	~CDatabaseMysql(void);

	bool Initialize(const std::string& host, const string& user, const string& pwd, const string& dbname);
	QueryResult* Query(const char *sql);
	QueryResult* Query(const std::string& sql)
	{
	    return Query(sql.c_str());
    }

	QueryResult* PQuery(const char *format,...);
	bool Execute(const char* sql);
	bool Execute(const char* sql, uint32_t& uAffectedCount, int& nErrno);
	bool PExecute(const char *format,...);

	uint32_t GetInsertID();

	void ClearStoredResults();

	int32_t EscapeString(char* szDst, const char* szSrc, uint32_t uSize);

private:
	DatabaseInfo m_DBInfo;
	MYSQL *m_Mysql;
	bool m_bInit;
};
