#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "iostream"
#include "sqlite3.h"
#include <tuple>
#include <list>
#include <memory.h>

#define COLUMN_LIST list<tuple<Type,int,string, double>>
#define ROW_LIST list<list<tuple<Type,int,string, double>>>

using namespace std;

class Database
{   
public:
    Database(string path);
    ~Database();

    enum Type{Integer, Int, Text, Real, Float, Double, Date, DateTime, Boolean, Varchar, Character};

    int query(string query, list<tuple<Type,int,string, double>>&data);
    list<list<tuple<Type,int,string, double>>> query(const char* fmt...);
    tuple<Type,int,string,double> makeTuple(int);
    tuple<Type,int,string,double> makeTuple(string);
    tuple<Database::Type,int,string,double> makeTuple(double value);

private:
    sqlite3 *mDb;
};


#endif
