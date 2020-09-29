#include "Database.h"

namespace Database{

    static list<list<pair<string,string>>> gResult;

    // static int callback(void *notUsed, int arg, char **argvs, char **azColName){ return 0; }
    static int callback(void *NotUsed, int argc, char **argv, char **azColName){
        int i;
        list<pair<string,string>> values;
        for(i=0; i<argc; i++){
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
            pair<string,string> p (azColName[i], argv[i] ? argv[i] : "NULL");
            values.push_back(p);
        }
        gResult.push_back(values);
        printf("\n");
        return 0;
    }


    /************************************* Sqlite *********************************************/
    Sqlite::Sqlite():mPath(""), mOpenFlag(false), mOk(true)
    {}

    Sqlite::Sqlite(string path):mPath(path)
    {
        if(sqlite3_open(path.c_str(), &mDb) != SQLITE_OK) {
            mOpenFlag = false;
            mOk = false;
            return ;
        }
        mOpenFlag = true;
        mOk = true;
    }


    Sqlite::~Sqlite()
    {
        sqlite3_close(mDb);
    }

    list<string> Sqlite::getColumnName()
    {
        return mColumnName;
    }

    list<string> Sqlite::getColumnName(string columnName)
    {
        string q = "PRAGMA table_info(" + columnName + ");";
        list<string> values;
        int ret = exec(q.c_str());
        if(ret != SQLITE_OK)
            return values;
        for(auto& l:gResult)
            for(auto& i:l){
                if(i.first == "name")
                    values.push_back(i.second);
            }
        return values;
    }

    list<string> Sqlite::getColumnValues(uint i)
    {   
        list<string> tmp;
        try
        {
             tmp = at(mRowValues, i);
        }
        catch(const std::exception& e)
        {
            setErr(e.what());
        }
        
        return tmp;
    }

    list<list<string>> Sqlite::getRowValues()
    {
        return mRowValues;
    }

    /**
     * function: int query(string query, list<tuple<Type,int,string, double>>&data)
     * 
     * description: 
     **/
    int Sqlite::query(string query, COLUMN_LIST&data)
    {
        sqlite3_stmt *stmt;
        char *errMsg;
        int ret;
        uint index = 1;
        
        mMutex.lock();
        gResult.clear();
        ret = sqlite3_prepare_v2(mDb,query.c_str(), query.size(), &stmt, 0);
        if(ret != SQLITE_OK){
            setErr(sqlite3_errmsg(mDb));
            return -1;
        }

        if(query.find("select") != string::npos || query.find("SELECT") != string::npos ||
           query.find("PRAGMA") != string::npos || query.find("pragma") != string::npos)
        {
            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                uint cnt = sqlite3_column_count(stmt);
                ret = cnt;
                list<pair<string,string>> values;
                for(int i=0; i<cnt; i++)
                {
                    pair<string,string> p;

                    // get column name
                    string type = sqlite3_column_decltype(stmt, i);
                    p.first = sqlite3_column_name(stmt, i);
                    cout << "column name: " << p.first << endl;
                    // get all values as a string
                    sqlite3_value *v = sqlite3_column_value(stmt, i);
                    p.second = (char*)(sqlite3_value_text(v));
                    cout << "value: " << p.second << endl;
                    cout << "type: " << sqlite3_value_type(v) << endl;

                    values.push_back(p);
                    

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

                gResult.push_back(values);
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
        mMutex.unlock();
        parseData();

        return ret;
    }


    /**
     *  function: list<list<SQL_DATA_TYPE>> query(const char* _args...)
     * 
     *  description: 
     */
    ROW_LIST Sqlite::query(const char* _args...)
    {
        string query = "";
        COLUMN_LIST data;
        ROW_LIST ret;
        va_list args;
        va_start(args, _args);
    
        while (*_args != '\0') 
        {
            if (*_args == 'q') {
                query = va_arg(args, char*);
                if(query.find("create") != string::npos || query.find("CREATE") != string::npos) {
                    exec(query);
                    return ret;
                }
            }
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

    bool Sqlite::open()
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
    bool Sqlite::close()
    {
        if(sqlite3_close(mDb) == SQLITE_OK){
            mOk = true;
            return true;
        }
        mOk = false;
        return false;
    }
    bool Sqlite::open(string _path)
    {
        mPath = _path;

        return open();
    }
    bool Sqlite::isOpen() const
    {
        return mOpenFlag;
    }
    bool Sqlite::isOk()
    {
        bool tmp = mOk;
        mOk = true;
        return tmp;
    }
    void Sqlite::setPath(string _path)
    {
        mPath = _path;
    }

    string Sqlite::getErrorMessage() const{
        return mErrMsg;
    }

    int Sqlite::exec(string query)
    {
        if(isOpen() == false)
            return -1;

        int ret = 0;

        mMutex.lock();

        char *errMsg = nullptr;
        gResult.clear();
        ret = sqlite3_exec(mDb, query.c_str(), callback, 0, &errMsg);
        if (ret != SQLITE_OK){
            ret = -1;
            setErr(errMsg);
        }
        else{
            clearErr();
            parseData();
        }
        mMutex.unlock();  

        return ret;
    }

    SQL_DATA_TYPE Sqlite::toSqlDataType(int value)
    {
        return make_tuple(Database::Type::Integer, value, "", 0.0);
    }
    SQL_DATA_TYPE Sqlite::toSqlDataType(string value)
    {
        return make_tuple(Database::Type::Text, 0, value, 0.0);
    }
    SQL_DATA_TYPE Sqlite::toSqlDataType(double value)
    {
        return make_tuple(Database::Type::Real, 0, "", value);
    }

    void Sqlite::setErr(string err)
    {
        mOk = false;
        mErrMsg = err;
    }

    void Sqlite::clearErr()
    {
        mOk = true;
        mErrMsg = "";
    }

    void Sqlite::parseData()
    {
        mRowValues.clear();
        mColumnName.clear();
        
        for(auto& cl:gResult){
            list<string> tmp;
            for(auto& i:cl){
                if(mColumnName.size() < cl.size())
                    mColumnName.push_back(i.first);
                
                tmp.push_back(i.second);
            }
            mRowValues.push_back(tmp);
        }
    }

/********************************** Tools ***************************************/

    list<string> split(string str, string sep)
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

    string &at(list<string> &target, uint pos)
    {
        if(pos >= target.size()){
            throw std::length_error("pos out of size");
        }
        list<string>::iterator it = target.begin();
        uint i = 0;
        for(; it != target.end(); it++, i++){
            if(i == pos){
                return *it;
            }
        }
        return *(--it);
    }

    list<string> &at(list<list<string>> &target, uint pos)
    {
        if(pos >= target.size()){
            throw std::length_error("pos out of size");
        }
        list<list<string>>::iterator it = target.begin();
        uint i = 0;
        for(; it != target.end(); it++, i++){
            if(i == pos){
                return *it;
            }
        }
        return *(--it);
    }

    COLUMN_LIST &at(ROW_LIST &target, uint pos)
    {
        if(pos >= target.size()){
            throw std::length_error("pos out of size");
        }
        ROW_LIST::iterator it = target.begin();
        uint i = 0;
        for(; it != target.end(); it++, i++){
            if(i == pos){
                return *it;
            }
        }
        return *(--it);
    }

    SQL_DATA_TYPE &at(COLUMN_LIST &target, uint pos)
    {
        if(pos >= target.size()){
            throw std::length_error("pos out of size");
        }
        COLUMN_LIST::iterator it = target.begin();
        uint i = 0;
        for(; it != target.end(); it++, i++){
            if(i == pos){
                return *it;
            }
        }
        return *(--it);
    }

    string toString(SQL_DATA_TYPE _data)
    {
        if(get<0>(_data) == Database::Text)
            return get<2>(_data);
        return "";
    }

    double toDouble(SQL_DATA_TYPE _data)
    {
        if(get<0>(_data) == Database::Real)
            return get<3>(_data);
        return 0;
    }

    int toInt(SQL_DATA_TYPE _data)
    {
        if(get<0>(_data) == Database::Integer)
            return get<1>(_data);
        return 0;
    }
}

