#include <iostream>
#include "Database.h"
#include <chrono>
using namespace std;

int main(int argc, char **argv)
{
	Database::Sqlite db;
	db.open("./test.db");
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
	// int data = db.exec("create table Application(ID integer primary key autoincrement, num int);");
	// if(data == -1)
	// 	cout << db.getErrorMessage() << endl;
	// */

	// db.query("q", "create table Test(num int, str text, num2 real);");
	// if(!db.isOk())
	// 	cout << db.getErrorMessage() << endl;
	// db.exec("insert into Test(num, str, num2) values(3, '3', 3.3);insert into Test(num, str, num2) values(4, '4', 4.4);");
	// if(!db.isOk())
	// 	cout << db.getErrorMessage() << endl;
	// ROW_LIST rl = db.query("q", "select * from Test;");
	// if(!db.isOk())
	// 	cout << db.getErrorMessage() << endl;
	// try {
	// 		for(auto& cl:rl){
	// 		cout << Database::toDouble(Database::at(cl, 4)) << endl;;
	// 	}
	// }
	// catch (const std::exception& e){
	// 	cout << e.what() << endl;
	// }

	// list<string> values = db.getColumnName("Test");
	// for (auto& i:values)
	// {
	// 	cout << i << endl;
	// }

	// list<string> values2 = db.getColumnName("AA");
	// for (auto& i:values2)
	// {
	// 	cout << i << endl;
	// }

	// list<string> values3 = db.getColumnName("Fault");
	// for (auto& i:values3)
	// {
	// 	cout << i << endl;
	// }

	chrono::system_clock::time_point tStart1 = chrono::system_clock::now();
	ROW_LIST rl; 
	db.query("q", "select * from Test;");
	if(!db.isOk())
		cout << db.getErrorMessage() << endl;
	chrono::system_clock::time_point tEnd1 = chrono::system_clock::now();
	cout << "elapse: " << chrono::duration_cast<chrono::microseconds>(tEnd1-tStart1).count() << endl;
	// for(auto& i:at(rl, 0)){
	// 	Database::Type type = get<0>(i);
	// 	if(type == Database::Integer)
	// 		cout << Database::toInt(i) << endl;
	// 	else if(type == Database::Text)
	// 		cout << Database::toString(i) << endl;
	// 	else if(type == Database::Real)
	// 		cout << Database::toDouble(i) << endl;
	// }
	list<list<string>> values5 = db.getRowValues();
	for (auto& cl:values5)
	{
		for(auto& i:cl)
			cout << i  << "|";
		cout << "\n";
	}
	
	
	
	cout << "\n\n";

	chrono::system_clock::time_point tStart = chrono::system_clock::now();
	db.exec("select * from Test;");
	if(!db.isOk())
		cout << db.getErrorMessage() << endl;

	chrono::system_clock::time_point tEnd = chrono::system_clock::now();
	cout << "elapse: " << chrono::duration_cast<chrono::microseconds>(tEnd-tStart).count() << endl;


	list<string> values4 = db.getColumnValues(0);
	for (auto& i:values4)
	{
		cout << i << endl;
	}
	
	
	return 0;
}

