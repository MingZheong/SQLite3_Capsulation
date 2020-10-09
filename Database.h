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
#include <cstring>
#include <typeinfo>

#define PREPARE_ERR 1000
#define BIND_ERR    1001
#define EXEC_ERR    1002

using namespace std;

namespace Database{

    list<string> split(string str, string sep);

}
#endif
