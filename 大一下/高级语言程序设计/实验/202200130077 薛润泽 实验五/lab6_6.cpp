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
	char nm[20]="default";
	char loc[20]="default";
	char city[20]="default";
	int nmb=1;
	Employee emp[5]={Employee(nm,loc,city,nmb),Employee(nm,loc,city,nmb),Employee(nm,loc,city,nmb),Employee(nm,loc,city,nmb),Employee(nm,loc,city,nmb)}; 
	char a[20]="empchange_name";
	for(int i=0;i<5;i++){
		emp[i].change_name(a);
		emp[i].display();
	}
	return 0;
}
