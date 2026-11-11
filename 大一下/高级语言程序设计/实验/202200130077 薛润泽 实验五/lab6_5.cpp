#include<iostream>
#include<string>
#include<string.h>
using namespace std;

class Employee{
	public:
		Employee(){};
		Employee(const char *nm,const char *loc,const char*ct,const int num);
		void display();
		void change_name(const char*nm);
	private:
		char name[20];
		char location[20];
		char city[20];
		int number;
};
Employee::Employee(const char*nm,const char*loc,const char*ct,const int num){
	strcpy(name,nm);
	strcpy(location,loc);
	strcpy(city,ct);
	number=num;
}
void Employee::change_name(const char*nm){
	strcpy(name,nm);
}
void Employee::display(){
	cout<<"姓名："<<name<<endl;
	cout<<"街道："<<location<<endl;
	cout<<"城市："<<city<<endl;
	cout<<"邮编："<<number<<endl; 
	cout<<"\n"<<endl;
}
int main(){
	char name1[20]="JANE";
	char location1[20]="ShangDong";
	char city1[20]="QingDao";
	int number=123456;
	Employee person(name1,location1,city1,number);
	person.display();
	char name2[20]="BLANGE"; 
	person.change_name(name2);
	person.display();
	return 0;
}
