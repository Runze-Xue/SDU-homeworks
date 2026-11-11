#include<iostream>
 
class Student{
public:
	Student(){
		name = new char[20];
		phonenumber = new char[20];
		cls = new int;
		dorm = new int ;
		std::cin>>name>>phonenumber>>*cls>>*dorm;
	}					//每new一个Student对象，都会调用构造函数输入该student的name, phonenumber class), drom 
	~Student(){
		delete []name;
		delete[]phonenumber;
		delete cls;
		delete dorm;	//对象析构要把所有new出的空间释放掉 
	}
	char * name;		//姓名 
	char * phonenumber; //电话号码 
	int * cls;		    //存放班级 
	int  *dorm;			//宿舍 
};
class AddressBook{
public:
	AddressBook(int n){
		people = new Student*[n];	//因为有n个操作，所以在n个操作中最多插入n个Student对象 
		cnt = 0;					//将cnt初始为0 
	}
	~AddressBook(){
		delete []people;			//删除指针数组	
	}
	void addressinsert();
	int studentnamecheck(char* A, char* B);//根据姓名，检查传入的两个姓名是否相等，如果相等则返回 
	int addressfind();			//根据姓名查找people数组中与满足姓名的student，返回索引 
	void addressdelete();		
	void addressupdate();
	int addressdormcal();
private: 
	Student** people;			//people是一个Student类型二维数组 
	int cnt;					//cnt记录 people数组中实际元素个数
};

void AddressBook::addressinsert(){

	people[cnt] = new Student() ; //构造函数自动调用，同时写入姓名等信息 
	cnt++;
}
int AddressBook::studentnamecheck(char *A, char* B){	// 检查操作名字和对应学生名字，两个名字是否相同，相同就返回1，不同就返回0 
	int sym = 0;
	while(A[sym]==B[sym]){
		if(A[sym]=='\0'&&B[sym] =='\0') return 1;
		sym++;
	}
	return  0;
}	
int AddressBook::addressfind(){							// 根据姓名查找 
	char* tempname = new char[20];
	std::cin>>tempname;									//输入要查找的姓名 
	for(int i = 0; i < cnt; i++){						 
		if(studentnamecheck(tempname,people[i]->name)){ //从数组第一个元素开始寻找，直到找到目标元素或者到最后一个元素 
			delete[]tempname;							//删除输入查找的名字 
			return i;									//找到了则返回查找到的索引！！索引 
		}
	}
	delete []tempname;
	return -1;											//没找到，输出-1
}

void AddressBook::addressdelete(){
	int location = addressfind();						//记录要被删除元素的索引 
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
	int location = addressfind();						//记录要被修改元素的索引 
	int sym = 0;
	std::cin>>sym;										//记录更新操作类型 
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
int AddressBook::addressdormcal(){						//计算宿舍异或值 
	int classnum;
	std::cin>>classnum;
	int tempans = 0;
	for(int i = 0; i < cnt; i++){						//判断该学生是否满足班级要求 
		if(*people[i]->cls == classnum)
		tempans = tempans^*(people[i]->dorm);
	}
	return tempans;
} 
int main(){
	int n;
	std::cin>>n;
	AddressBook addressbook(n);
	int opt;
	for(int i = 0; i < n; i++){
		std::cin>>opt;
		if(opt == 0){
			addressbook.addressinsert();
		}
		else if(opt == 1){
			addressbook.addressdelete();
		}
		else if(opt == 2){
			addressbook.addressupdate();
		}
		else if(opt == 3){
			int ans = addressbook.addressfind();
			if(ans == -1) std::cout<<'0'<<'\n';
			else std::cout<<'1'<<'\n'; 
		}
		else if(opt == 4){
			int ans = addressbook.addressdormcal();
			std::cout<<ans<<'\n';
		}
	}
	return 0;
}
