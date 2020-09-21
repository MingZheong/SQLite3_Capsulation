#include <iostream>
#include "Database.h"


using namespace std;

int initializerSum(initializer_list<int> il) {
   
	int sum = 0;
 
	for (auto ptr = il.begin(); ptr != il.end(); ptr++)  //类似于容器的操作
	{
		sum += *ptr;
	}
 
	return sum;
}


void f2(char* fmt, ...)

{
    va_list ap;
    va_start(ap, fmt);
    char*p = fmt;
    while(*p) {
       if(*p == '%'&& *(p+1) == 'd') {
           printf("参数类型为int,值为%d\n", va_arg(ap, int));
       }
       else if (*p == '%' && *(p+1)== 'f') {
           printf("参数类型为double,值为%f\n", va_arg(ap, double));
       }
       else if (*p == '%' && *(p+1)== 's') {
           printf("参数类型为char*,值为%s\n", va_arg(ap, char*));
       }
       p++;
    }
    va_end(ap);

}

int main(int argc, char **argv)
{
	
	Database db("/home/linux/Desktop/test/sql/test.db");
	/* 
	// use query and values to handle database
	list<tuple<Database::Type, int, string>>data;
	
	// test insert
	// string query = "insert into Test2(Data, Num, Str) values(?, ?, ?);";
	// data.push_back(db.makeTuple("what the hell?"));
	// data.push_back(db.makeTuple(888));
	// data.push_back(db.makeTuple("bey"));
	
	// test select
	// string query = "select * from Test2;";
	
	// test delete
	// string query = "delete from Test2 where ID=3;";
	
	// test update
	string query = "update Test2 set Num=?, Str=? where ID=4;";
	data.push_back(db.makeTuple(999));
	data.push_back(db.makeTuple("888"));


	db.query(query, data);

	for(auto& item:data)
		(get<0>(item) == Database::Type::Integer)?(cout << get<1>(item) << endl):(cout << get<2>(item) << endl);
	*/

	// use variable element function to handle database
	list<list<tuple<Database::Type,int,string, double>>> data = db.query("q", "select * from Test2;");

	for(auto& item :data){
		for(auto&i:item){
			if(get<0>(i) == Database::Type::Text)
				cout  << get<2>(i);
			else if(get<0>(i) == Database::Type::Integer)
				cout  << get<1>(i);
			else if(get<0>(i) == Database::Type::Double)
				cout  << get<3>(i);
			cout << "|";
		}
		cout << endl;
	}

	return 0;
}

