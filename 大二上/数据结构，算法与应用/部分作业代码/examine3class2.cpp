#include<iostream>
//对于插入操作，new一个Student对象，然后，利用构造函数改变姓名等信心
 
class Student{
public:
	Student(){
		name = new char[20];
		phonenumber = new char[20];
		cls = new int;
		dorm = new int ;
		std::cin>>name>>phonenumber>>*cls>>*dorm;
	}
	~Student(){
		delete []name;
		delete[]phonenumber;
		delete cls;
		delete dorm;
	}
	char * name;		//姓名 
	char * phonenumber; //电话号码 
	int * cls;		    //存放班级 
	int  *dorm;		//宿舍 
};
class AddressBook{
public:
	AddressBook(int n){
		people = new Student*[n];
		cnt = 0;
	}
	~AddressBook(){
		delete []people;
	}
	void addressinsert();
	int studentnamecheck(char* A, char* B);//根据姓名，检查传入的两个姓名是否相等，如果相等则返回 
	int addressfind();			//根据姓名查找people数组中与满足姓名的student，返回索引 
	void addressdelete();
	void addressupdate();
	int addressdormcal();
	
	Student** people;
	int cnt;					//cnt记录 people数组中实际元素个数
};

void AddressBook::addressinsert(){

	people[cnt] = new Student() ; //构造函数自动调用，同时写入姓名等信息 
	cnt++;
//	std::cout<<"End";
}
int AddressBook::studentnamecheck(char *A, char* B){	
	int sym = 0;
	while(A[sym]==B[sym]){
		if(A[sym]=='\0'&&B[sym] =='\0') return 1;
		sym++;
	}
	return  0;
}
int AddressBook::addressfind(){
	char* tempname = new char[20];
	std::cin>>tempname;
	for(int i = 0; i < cnt; i++){
		if(studentnamecheck(tempname,people[i]->name)){
			delete[]tempname;
			return i;
		}
	}
	delete []tempname;
	return -1;				//没找到，输出-1
}

void AddressBook::addressdelete(){
	int location = addressfind();
	if(location==-1) std::cout<<"Wrong name";
	else {
		delete people[location];
		for(int i = location; i < cnt - 1; i++){
			people[i] = people[i + 1];
			people[i + 1] =nullptr;
		}	
	}
	cnt--;
}

void AddressBook::addressupdate(){
	int location = addressfind();
	int sym = 0;
	std::cin>>sym;
	if(sym == 1){
		std::cin>>people[location]->phonenumber;
	}
	else if(sym == 2){
		std::cin>>*people[location]->cls;
	}
	else if(sym == 3){
		std::cin>>*(people[location]->dorm);
	}
}
int AddressBook::addressdormcal(){
	int classnum;
	std::cin>>classnum;
	int tempans = 0;
	for(int i = 0; i < cnt; i++){
		if(*people[i]->cls == classnum)
		tempans = tempans^*(people[i]->dorm);
	}
	return tempans;
} 
int main(){
	int n;
	std::cin>>n;
	AddressBook addressbook(n);
	addressbook.addressinsert();
	addressbook.addressinsert();
	int temp = addressbook.addressdormcal();
	std::cout<<temp;
//	std::cout<<addressbook.addressdormcal();
//	int opt[28] = {0,0,3,4,3,1,4,1,3,3,3,0,4,4,3,3,2,3,2,1,3,3,0,3,1,0,1,3};
//	for(int i = 0; i < n; i++){
////		std::cin>>opt;
//		if(opt[i] == 0){
//			addressbook.addressinsert();
//		}
//		else if(opt[i] == 1){
//			addressbook.addressdelete();
//		}
//		else if(opt[i] == 2){
//			addressbook.addressupdate();
//		}
//		else if(opt[i] == 3){
//			int ans = addressbook.addressfind();
//			std::cout<<ans<<'\n';
//		}
//		else if(opt[i] == 4){
//			int ans = addressbook.addressdormcal();
//			std::cout<<ans<<'\n';
//		}
//	}
	return 0;
}
