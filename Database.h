/**
 *  A capsulation of sqlite3.
 *  
 *  Support types: Integer, Int, Real, Float, Boolean, Date, Character, VarChar, Real, Text
 * 
 *  author: Zhang Ming
 * 
 */

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "iostream"
#include "sqlite3.h"
#include <tuple>
#include <list>
// #include <memory.h>

#define SQL_DATA_TYPE tuple<Database::Type,int,string,double>
#define COLUMN_LIST list<SQL_DATA_TYPE>
#define ROW_LIST list<COLUMN_LIST>


using namespace std;

class Database
{   
public:
    Database();
    Database(string path);
    ~Database();

    enum Type{Integer, Text, Real};

    bool open();
    bool close();
    bool open(string);
    bool isOpen() const;
    bool isOk();
    void setPath(string);

    int query(string query, COLUMN_LIST &data);
    ROW_LIST query(const char* fmt...);
    SQL_DATA_TYPE toSqlDataType(int);
    SQL_DATA_TYPE toSqlDataType(short);
    SQL_DATA_TYPE toSqlDataType(string);
    SQL_DATA_TYPE toSqlDataType(float);
    SQL_DATA_TYPE toSqlDataType(double);

    static string toString(SQL_DATA_TYPE);
    static double toDouble(SQL_DATA_TYPE);
    static int toInt(SQL_DATA_TYPE);

private:
    sqlite3 *mDb;
    bool mOpenFlag;
    string mPath;
    bool mOk;
};


static list<string> split(string str, string sep)
{
    list<string> strList;
    uint startPos = 0;
    while (1)
    {
        if(startPos == str.size()+sep.size()) break;
        uint pos = str.find(sep,startPos);
        //cout << pos << endl;
        if(pos == string::npos || pos >= str.size())
            pos = str.size();
        string tmp = str.substr(startPos, pos-startPos);
        strList.push_back(tmp);
        startPos = pos+sep.size();
    }
    return strList;
}

#endif
