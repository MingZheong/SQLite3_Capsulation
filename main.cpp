#include <iostream>
#include "Database.h"


using namespace std;

int main(int argc, char **argv)
{
	Database db;
	db.open("/home/linux/Desktop/test/sql/test.db");
	// Database db("/home/linux/Desktop/test/sql/test.db");
	/* 
	// use query and values to handle database
	list<tuple<Database::Type, int, string>>data;
	
	// test insert
	// string query = "insert into Test2(Data, Num, Str) values(?, ?, ?);";
	// data.push_back(db.toSqlDataType("what the hell?"));
	// data.push_back(db.toSqlDataType(888));
	// data.push_back(db.toSqlDataType("bey"));
	
	// test select
	// string query = "select * from Test2;";
	
	// test delete
	// string query = "delete from Test2 where ID=3;";
	
	// test update
	string query = "update Test2 set Num=?, Str=? where ID=4;";
	data.push_back(db.toSqlDataType(999));
	data.push_back(db.toSqlDataType("888"));


	db.query(query, data);

	for(auto& item:data)
		(get<0>(item) == Database::Type::Integer)?(cout << get<1>(item) << endl):(cout << get<2>(item) << endl);
	*/

	// /*
	// use variable element function to handle database
	// ROW_LIST data = db.query("q", "select name from sqlite_master where type='table';");
	ROW_LIST data = db.query("q", "select * from Test3;");
	cout << data.size() << endl;
	for(auto& item :data){
		cout  << "--" << item.size() << endl;
		for(auto&i:item){
			cout << Database::toString(i);
			cout << "|";
		}
		cout << endl;
	}
	// */

	// db.query("qi", "insert into Test3(Data,Num,Num2,Num3,Num4,Num5,Str,Str2,Str3)")

	return 0;
}

