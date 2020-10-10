#include "Sqlite.h"

namespace Database{

static querys gResult;


/**
 * callback function
*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName){

    query q;
    for(int i=0; i<argc; i++){
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        pair<string,string> p(azColName[i], argv[i]);
        q.insert(p);
        
    }
    gResult.push_back(q);
    // printf("\n");
    return 0;
}


/************************************* Sqlite *********************************************/

/**
 * constructor
*/
Sqlite::Sqlite():mPath(""), mOpenFlag(false)
{

}

/**
 * copy constructor
*/
Sqlite::Sqlite(string path):mPath(path)
{
    if(sqlite3_open(path.c_str(), &mDb) != SQLITE_OK) {
        mOpenFlag = false;
        return ;
    }
    mOpenFlag = true;
}

/**
 * destructor
*/
Sqlite::~Sqlite()
{
    sqlite3_close(mDb);
}

/**
 * 
*/
bool Sqlite::open()
{
    if(!mOpenFlag){
        mErr.errCode = OPEN_ERR;
        mErr.errMsg = "databse has been opened";
        return false;
    }
    if(mPath.size() == 0){
        mErr.errCode = OPEN_ERR;
        mErr.errMsg = "databse peth not exist";
        mOpenFlag = false;
        return false;
    }
    if(sqlite3_open(mPath.c_str(), &mDb) != SQLITE_OK){
        mErr.errCode = OPEN_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        mOpenFlag = false;
        return false;
    }
    mOpenFlag = true;
    return true;
}

/**
 * 
*/
bool Sqlite::open(string _path)
{
    mPath = _path;

    return open();
}

/**
 * 
*/
bool Sqlite::close()
{
    if(sqlite3_close(mDb) == SQLITE_OK){
        mOpenFlag = false;
        return true;
    }

    return false;
}

/**
 * 
*/
bool Sqlite::isOpen() const
{
    return mOpenFlag;
}

/**
 * 
*/
void Sqlite::setPath(string _path)
{
    mPath = _path;
}


/************************************* use exec ****************************************/


/**
 * 
*/
int Sqlite::exec(string query)
{
    if(!mOpenFlag){
        mErr.errCode = OPEN_ERR;
        mErr.errMsg = "databse is closed";
        return -1;
    }

    mDbMutex.lock();

    char *errMsg = nullptr;
    gResult.clear();
    int ret = sqlite3_exec(mDb, query.c_str(), callback, 0, &errMsg);
    if (ret != SQLITE_OK){
        mErr.errCode = EXEC_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        ret = EXEC_ERR;
    }

    mDbMutex.unlock();  

    return ret;
}




/************************************* use prepare and bind ****************************************/



/**
 * 
*/
int Sqlite::prepare(string query)
{
    if(!mOpenFlag){
        mErr.errCode = OPEN_ERR;
        mErr.errMsg = "databse is closed";
        return -1;
    }

    mDbMutex.lock();
    mPrepare = true;

    int ret = sqlite3_prepare_v2(mDb, query.c_str(), query.size(), &mStmt, 0);
    if(ret != SQLITE_OK){
        mErr.errCode = PREPARE_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        mPrepare = false;
        mDbMutex.unlock();
        return PREPARE_ERR;
    }
    mStmtIndex = 1;
    
    return 0;
}

/**
 * 
*/
int Sqlite::bind(int value)
{
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_int(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        sqlite3_clear_bindings(mStmt);
        sqlite3_finalize(mStmt);
        mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(short value){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_int(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;

}

/**
 * 
*/
int Sqlite::bind(long value){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_int64(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(long long value){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_int64(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(float value){
    if(!mPrepare)
        return PREPARE_ERR;
   
    int ret = sqlite3_bind_double(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(double value){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_double(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(char* value){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_text(mStmt, mStmtIndex++, value, strlen(value), 0);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(string value){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_text(mStmt, mStmtIndex++, value.c_str(), value.size(), 0);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(void* value, int size){
    if(!mPrepare)
        return PREPARE_ERR;

    int ret = sqlite3_bind_blob(mStmt, mStmtIndex++, value, size, 0);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::binds(const char* fmt...){
    if(!mPrepare)
        return PREPARE_ERR;
    
    return 0;
}

/**
 * 
*/
int Sqlite::bind_finish(){
    if(!mPrepare)
        return PREPARE_ERR;

    mPrepare = false;

    sqlite3_step(mStmt);
    sqlite3_reset(mStmt);

    int ret = sqlite3_finalize(mStmt);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
    }
    mDbMutex.unlock();
    return 0;
}

querys Sqlite::getQuerys()
{
    if(mPrepare == false)
        return gResult;
    mPrepare = false;
    querys qs;
    while(sqlite3_step(mStmt) == SQLITE_ROW)
    {
        uint cnt = sqlite3_column_count(mStmt);
        query values;
        for(int i=0; i<cnt; i++)
        {
            pair<string,string> p;

            // get column name
            string type = sqlite3_column_decltype(mStmt, i);
            p.first = sqlite3_column_name(mStmt, i);
            
            // get all values as a string
            sqlite3_value *v = sqlite3_column_value(mStmt, i);
            p.second = (char*)(sqlite3_value_text(v));

            values.insert(p);
        }
        qs.push_back(values);
    }
    sqlite3_finalize(mStmt);
    mDbMutex.unlock();
    return qs;
}

const SQLError &Sqlite::lastError()
{
    return mErr;
}


}

