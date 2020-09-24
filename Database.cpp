#include "Database.h"


Database::Database():mPath(""), mOpenFlag(false), mOk(true)
{}

Database::Database(string path):mPath(path)
{
    if(sqlite3_open(path.c_str(), &mDb) != SQLITE_OK){
        mOpenFlag = false;
        mOk = false;
        return ;
    }
    mOpenFlag = true;
    mOk = true;
}


Database::~Database()
{
    sqlite3_close(mDb);
}

/**
 * function: int query(string query, list<tuple<Type,int,string, double>>&data)
 * 
 * description: 
 **/
int Database::query(string query, COLUMN_LIST&data)
{
    sqlite3_stmt *stmt;
    char *errMsg;
    int ret;
    uint index = 1;
    
    ret = sqlite3_prepare_v2(mDb,query.c_str(), query.size(), &stmt, 0);
    if(ret != SQLITE_OK){
        mOk = false;
        return -1;
    }

    if(query.find("select") != string::npos || query.find("SELECT") != string::npos)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            uint cnt = sqlite3_column_count(stmt);
            ret = cnt;
            for(int i=0; i<cnt; i++)
            {
                string type = sqlite3_column_decltype(stmt, i);
                if(type.find("int") != string::npos || type.find("INT") != string::npos){
                    data.push_back(toSqlDataType(sqlite3_column_int(stmt,i)));
                }
                else if(type.find("char") != string::npos || type.find("CHAR") != string::npos ||
                        type.find("TEXT") != string::npos || type.find("text") != string::npos){
                    data.push_back(toSqlDataType(string((char*)sqlite3_column_text(stmt,i))));
                }
                else if(type.compare("float") == 0 || type.compare("FLOAT") == 0 ||
                        type.compare("double") == 0 || type.compare("DOUBLE") == 0 || 
                        type.compare("real") == 0 || type.compare("REAL") == 0){
                    data.push_back(toSqlDataType(sqlite3_column_double(stmt,i)));
                }
                else if(type.compare("datetime") == 0 || type.compare("DATETIME") == 0 || 
                        type.compare("date") == 0 || type.compare("DATE") == 0){
                    data.push_back(toSqlDataType((char *)sqlite3_column_text(stmt,i)));
                }
            }
        }
    }
    else if(query.find("insert") != string::npos || query.find("INSERT") != string::npos || 
            query.find("update") != string::npos || query.find("UPDATE") != string::npos)
    {
        for(auto& item:data)
        {
            switch (get<0>(item))
            {
            case Type::Integer:
                sqlite3_bind_int(stmt, index++, get<1>(item));
                break;
            case Type::Text:
                sqlite3_bind_text(stmt, index++, get<2>(item).c_str(), get<2>(item).size(), 0);
                break;
            case Type::Real:
                sqlite3_bind_double(stmt, index++, get<3>(item));
                break;
            default:
            mOk = false;
                break;
            } 
        }

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    else if(query.find("delete") != string::npos || query.find("DELETE") != string::npos)
    {
        sqlite3_step(stmt);
    }
    
    sqlite3_finalize(stmt);
    
    return ret;
}


/**
 *  function: list<list<SQL_DATA_TYPE>> query(const char* _args...)
 * 
 *  description: 
 */
ROW_LIST Database::query(const char* _args...)
{
    string query = "";
    COLUMN_LIST data;

    va_list args;
    va_start(args, _args);
 
    while (*_args != '\0') 
	{
        if (*_args == 'q') 
            query = va_arg(args, char*);
		else if (*_args == 'i') 
		{
            int i = va_arg(args, int);
            data.push_back(toSqlDataType(i));
        } 
        else if (*_args == 'd')
		{
            double d = va_arg(args, double);
            data.push_back(toSqlDataType(d));
        }
		else if(*_args == 's')
		{
			string s = va_arg(args, char*);
            data.push_back(toSqlDataType(s));
		}
        ++_args;
    }
    va_end(args);

    ROW_LIST ret;
    COLUMN_LIST tmp;
    int columnNum = this->query(query, data);
    if(columnNum != 0) {
        int inc = data.size()/columnNum;
        int index = 0;
        for(auto& item:data){
            tmp.push_back(item);
            index++;
            if(index == columnNum){
                ret.push_back(tmp);
                tmp.clear();
                index = 0;
            }
        }
    }
    return ret;
}

bool Database::open()
{
    if (mPath.size() == 0){
        mPath = "";
        mOpenFlag = false;
        mOk = false;
        return false;
    }
    if(sqlite3_open(mPath.c_str(), &mDb) != SQLITE_OK){
        mOpenFlag = false;
        mOk = false;
        return false;
    }
    mOpenFlag = true;
    mOk = true;
    return true;
}
bool Database::close()
{
    if(sqlite3_close(mDb) == SQLITE_OK){
        mOk = true;
        return true;
    }
    mOk = false;
    return false;
}
bool Database::open(string _path)
{
    mPath = _path;

    return open();
}
bool Database::isOpen() const
{
    return mOpenFlag;
}
bool Database::isOk()
{
    bool tmp = mOk;
    mOk = true;
    return tmp;
}
void Database::setPath(string _path)
{
    mPath = _path;
}

SQL_DATA_TYPE Database::toSqlDataType(int value)
{
    return make_tuple(Database::Type::Integer, value, "", 0.0);
}
SQL_DATA_TYPE Database::toSqlDataType(string value)
{
    return make_tuple(Database::Type::Text, 0, value, 0.0);
}
SQL_DATA_TYPE Database::toSqlDataType(double value)
{
    return make_tuple(Database::Type::Real, 0, "", value);
}

string Database::toString(SQL_DATA_TYPE _data)
{
    if(get<0>(_data) == Database::Text)
        return get<2>(_data);
    return "";
}
double Database::toDouble(SQL_DATA_TYPE _data)
{
    if(get<0>(_data) == Database::Real)
        return get<3>(_data);
    return 0;
}
int Database::toInt(SQL_DATA_TYPE _data)
{
    if(get<0>(_data) == Database::Integer)
        return get<1>(_data);
    return 0;
}
