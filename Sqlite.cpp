#include "Sqlite.h"

namespace Database{

static querys gResult;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){

    query q;
    for(int i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        pair<string,string> p(azColName[i], argv[i]);
        q.insert(p);
        
    }
    gResult.push_back(q);
    printf("\n");
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
    if (mPath.size() == 0){
        mPath = "";
        mOpenFlag = false;
        return false;
    }
    if(sqlite3_open(mPath.c_str(), &mDb) != SQLITE_OK){
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
        return true;
    }

    mOpenFlag = false;
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

int Sqlite::exec(string query)
{
    if(isOpen() == false)
        return -1;

    int ret = 0;

    mDbMutex.lock();

    char *errMsg = nullptr;
    gResult.clear();
    ret = sqlite3_exec(mDb, query.c_str(), callback, 0, &errMsg);
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
    if (!mOpenFlag)
        return -1;
    mDbMutex.lock();
    int ret = sqlite3_prepare_v2(mDb, query.c_str(), query.size(), &mStmt, 0);
    if(ret != SQLITE_OK){
        mDbMutex.unlock();
        mPrepare = false;
        return -2;
    }
    mStmtIndex = 1;
    mPrepare = true;

    return 0;
}

/**
 * 
*/
int Sqlite::bind(int value)
{
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

    int ret = sqlite3_bind_int(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;

}

/**
 * 
*/
int Sqlite::bind(long value){

    int ret = sqlite3_bind_int64(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(long long value){

    int ret = sqlite3_bind_int64(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(float value){
   
    int ret = sqlite3_bind_double(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(double value){

    int ret = sqlite3_bind_double(mStmt, mStmtIndex++, value);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(char* value){

    int ret = sqlite3_bind_text(mStmt, mStmtIndex++, value, strlen(value), 0);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(string value){

    int ret = sqlite3_bind_text(mStmt, mStmtIndex++, value.c_str(), value.size(), 0);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::bind(void* value, int size){

    int ret = sqlite3_bind_blob(mStmt, mStmtIndex++, value, size, 0);
    if(ret != SQLITE_OK)
    {
        mErr.errCode = BIND_ERR;
        mErr.errMsg = sqlite3_errmsg(mDb);
        // sqlite3_clear_bindings(mStmt);
        // sqlite3_finalize(mStmt);
        // mDbMutex.unlock();
        return BIND_ERR;
    }
    return ret;
}

/**
 * 
*/
int Sqlite::binds(const char* fmt...){
    
    return 0;
}

/**
 * 
*/
int Sqlite::bind_finish()
{
    mPrepare = false;

    if(mErr.errCode == BIND_ERR){
        mErr.errCode = 0;
        mErr.errMsg = "";
        sqlite3_clear_bindings(mStmt);
        sqlite3_finalize(mStmt);
        mDbMutex.unlock();
        return -1;
    }

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
        cout << "-----" << cnt << "-----" << endl;
        query values;
        for(int i=0; i<cnt; i++)
        {
            pair<string,string> p;

            // get column name
            string type = sqlite3_column_decltype(mStmt, i);
            p.first = sqlite3_column_name(mStmt, i);
            cout << "column name: " << p.first << " ";
            
            // get all values as a string
            sqlite3_value *v = sqlite3_column_value(mStmt, i);
            p.second = (char*)(sqlite3_value_text(v));
            cout << "value: " << p.second << " ";
            cout << "type: " << sqlite3_value_type(v) << endl;

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

