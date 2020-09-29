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
#include <mutex>
#include <vector>

// #include <memory.h>

#define SQL_DATA_TYPE tuple<Database::Type,int,string,double>
#define COLUMN_LIST list<SQL_DATA_TYPE>
#define ROW_LIST list<COLUMN_LIST>

using namespace std;

namespace Database{

    enum Type{Integer, Text, Real};    
    class Sqlite
    {
    public:
        Sqlite();
        Sqlite(string path);
        ~Sqlite();

        list<string> getColumnName();
        list<string> getColumnName(string);
        list<string> getColumnValues(uint);
        list<list<string>> getRowValues();
        bool open();
        bool close();
        bool open(string);
        bool isOpen() const;
        bool isOk();
        void setPath(string);
        string getErrorMessage() const;

        int exec(string);

        int query(string query, COLUMN_LIST &data);
        ROW_LIST query(const char* fmt...);
        SQL_DATA_TYPE toSqlDataType(int);
        SQL_DATA_TYPE toSqlDataType(short);
        SQL_DATA_TYPE toSqlDataType(string);
        SQL_DATA_TYPE toSqlDataType(float);
        SQL_DATA_TYPE toSqlDataType(double);

    private:
        void setErr(string);
        void clearErr();
        void parseData();

    private:
        sqlite3 *mDb;
        bool mOpenFlag;
        string mPath;
        string mErrMsg;
        bool mOk;
        mutex mMutex;
        
        list<string>mColumnName;
        list<list<string>>mRowValues;
    };

    list<string> split(string str, string sep);
    string &at(list<string> &target, uint pos);
    list<string> &at(list<list<string>> &target, uint pos);
    
    COLUMN_LIST &at(ROW_LIST &target, uint pos);
    SQL_DATA_TYPE &at(COLUMN_LIST &target, uint pos);
    string toString(SQL_DATA_TYPE);
    double toDouble(SQL_DATA_TYPE);
    int toInt(SQL_DATA_TYPE);

}
#endif
