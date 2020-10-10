#include <iostream>
#include "Sqlite.h"
#include <chrono>


using namespace std;

int main(int argc, char **argv)
{
	Database::Sqlite db;
	db.open("./test.db");
	
	// int a;
	// short b;
	// long c;
	// long long d;
	// string e = "da";
	// char f;
	// char g[10];
	// float h;
	// double i;

	// cout << typeid(a).name() << endl;
	// cout << typeid(b).name() << endl;
	// cout << typeid(c).name() << endl;
	// cout << typeid(d).name() << endl;
	// cout << typeid(e).name() << endl;
	// cout << typeid(f).name() << endl;
	// cout << typeid(g).name() << endl;
	// cout << typeid(h).name() << endl;
	// cout << typeid(i).name() << endl;


chrono::system_clock::time_point tStart1 = chrono::system_clock::now();
	int ret = db.prepare("insert into Test(num,stdr,num2) values(?,?,?);");
	if (ret != 0)
		cout << "prepare fail" << endl;
	int data = 12;
	string str = "asdfasd";
	double num = 12.123123;

	ret = db.bind(data);
	if(ret != 0)
		cout << db.lastError().errMsg << endl;

	ret = db.bind(str);
	if(ret != 0)
		cout << db.lastError().errMsg << endl;

	ret = db.bind(num);
	if(ret != 0)
		cout << db.lastError().errMsg << endl;

	ret = db.bind_finish();
	if(ret != 0)
		cout << "err" << endl;
chrono::system_clock::time_point tEnd1 = chrono::system_clock::now();

cout << "elapse: " << chrono::duration_cast<chrono::microseconds>(tEnd1-tStart1).count() << endl;

chrono::system_clock::time_point tStart2 = chrono::system_clock::now();
	ret = db.exec("insert into Test(num,str,num2) values(123,'asdfasdf', 123123.12312);");
	if (ret != 0)
		cout << "err " << endl;
chrono::system_clock::time_point tEnd2 = chrono::system_clock::now();

cout << "elapse: " << chrono::duration_cast<chrono::microseconds>(tEnd2-tStart2).count() << endl;

chrono::system_clock::time_point tStart3 = chrono::system_clock::now();
	db.prepare("select * from Test;");
	db.getQuerys();
chrono::system_clock::time_point tEnd3 = chrono::system_clock::now();

cout << "elapse: " << chrono::duration_cast<chrono::microseconds>(tEnd3-tStart3).count() << endl;

chrono::system_clock::time_point tStart4 = chrono::system_clock::now();
	db.exec("select * from Test;");
	db.getQuerys();
chrono::system_clock::time_point tEnd4 = chrono::system_clock::now();

cout << "elapse: " << chrono::duration_cast<chrono::microseconds>(tEnd4-tStart4).count() << endl;

	return 0;
}

