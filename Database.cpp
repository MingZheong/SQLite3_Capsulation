#include "Database.h"

Database::Database(string path)
{
    if(sqlite3_open(path.c_str(), &mDb) != SQLITE_OK){
        cout << "open database<" << path << "> fail!";
        return ;
    }else
    {
        cout << "database open ok" << endl;
    }
    
}
/**
 * function: int query(string query, list<tuple<Type,int,string, double>>&data)
 * 
 * description: 
 **/
int Database::query(string query, list<tuple<Type,int,string,double>>&data)
{
    sqlite3_stmt *stmt;
    char *errMsg;
    int ret;
    uint index = 1;
    
    ret = sqlite3_prepare_v2(mDb,query.c_str(), query.size(), &stmt, 0);
    if(ret != SQLITE_OK){
        cout << "prepare " + query + " fail!";
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
                if(type.compare("integer") == 0 || type.compare("INTEGER") == 0){
                    data.push_back(makeTuple(sqlite3_column_int(stmt,i)));
                }
                else if(type.compare("text") == 0 || type.compare("TEXT") == 0){
                    data.push_back(makeTuple(string((char *)sqlite3_column_text(stmt,i))));
                }
                else if(type.compare("float") == 0 || type.compare("FLOAT") == 0){
                    data.push_back(makeTuple(sqlite3_column_double(stmt,i)));
                }
                else if(type.compare("double") == 0 || type.compare("DOUBLE") == 0){
                    data.push_back(makeTuple(sqlite3_column_double(stmt,i)));
                    cout << get<3>(data.back()) << endl;
                }
                else if(type.compare("datetime") == 0 || type.compare("DATETIME") == 0){
                    data.push_back(makeTuple((char *)sqlite3_column_text(stmt,i)));
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
            case Type::Double:
                sqlite3_bind_double(stmt, index++, get<3>(item));
                break;
            default:
                cout << "unknow Type:" << get<0>(item) << endl;
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
 *  function: list<list<tuple<Database::Type,int,string,double>>> query(const char* _args...)
 * 
 *  description: 
 */
list<list<tuple<Database::Type,int,string,double>>> Database::query(const char* _args...)
{
    string query = "";
    list<tuple<Database::Type, int, string, double>>data;

    va_list args;
    va_start(args, _args);
 
    while (*_args != '\0') 
	{
        if (*_args == 'q') 
            query = va_arg(args, char*);
		else if (*_args == 'd') 
		{
            int d = va_arg(args, int);
            data.push_back(makeTuple(d));
        } 
		else if (*_args == 'f')
		{
            double d = va_arg(args, double);
            std::cout << d << '\n';
        }
		else if(*_args == 's')
		{
			string s = va_arg(args, char*);
            data.push_back(makeTuple(s));
		}
        ++_args;
    }
    va_end(args);

    list<list<tuple<Database::Type,int,string,double>>>ret;
    list<tuple<Database::Type,int,string,double>>tmp;
    int columnNum = this->query(query, data);
    
    if(columnNum != 0) {
        int inc = data.size()/columnNum;
        int index = 0;
        for(auto& item:data){

            tmp.push_back(item);
            if(index == inc){
                ret.push_back(tmp);
                tmp.clear();
                index = -1;
            }
            index++;
        }
    }
    return ret;
}


tuple<Database::Type,int,string,double> Database::makeTuple(int value)
{
    return make_tuple(Database::Type::Integer, value, "", 0.0);
}

tuple<Database::Type,int,string,double> Database::makeTuple(string value)
{
    return make_tuple(Database::Type::Text, 0, value, 0.0);
}

tuple<Database::Type,int,string,double> Database::makeTuple(double value)
{
    return make_tuple(Database::Type::Double, 0, "", value);
}

Database::~Database()
{
    sqlite3_close(mDb);
}