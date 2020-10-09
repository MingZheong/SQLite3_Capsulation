/**
 *  A capsulation of sqlite3.
 *  
 *  Support types: Integer, Int, Real, Float, Boolean, Date, Character, VarChar, Real, Text
 * 
 *  author: Zhang Ming
 * 
 */

#ifndef _SQLITE_
#define _SQLITE_

#include "iostream"
#include "Database.h"
#include <tuple>
#include <list>
#include <mutex>
#include <vector>
#include <cstring>
#include <thread>
#include <map>
#include <sstream>

using namespace std;

namespace Database{

typedef map<string,string> query;
typedef list<map<string,string>> querys;

enum Type{Integer, Text, Real};


struct SQLError{
    int errCode;
    string errMsg;
};


class Sqlite
{
public:
    Sqlite();
    Sqlite(string path);
    ~Sqlite();

    bool open();
    bool open(string);
    bool close();
    bool isOpen() const;
    void setPath(string);

    int exec(string);

    int prepare(string);
    int bind(int);
    int bind(short);
    int bind(long);
    int bind(long long);
    int bind(float);
    int bind(double);
    int bind(char);
    int bind(char*);
    int bind(string);
    int bind(void*,int);
    int binds(const char* fmt...);
    int bind_finish();
    void clearBindings();

    querys getQuerys();

    const SQLError& lastError();

private:
    sqlite3* mDb;
    string mPath;
    mutex mDbMutex, mResultMutex;
    bool mOpenFlag;
    
    sqlite3_stmt *mStmt;
    int mStmtIndex;
    bool mPrepare;

    struct SQLError mErr;
};

}
#endif
