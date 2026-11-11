#include<iostream>
using namespace std;
enum SEX{boy=1,girl};

class DATE {
public: 
	DATE(int y, int m, int d);
	DATE(){}
	inline void showdate() {
		cout << "出生年：" << year << "出生月：" << month << "出生日：" << day;
	}
	DATE(DATE& q) {
		year = q.year;
		month = q.month;
		day = q.day;
	}
	~DATE() {};
private:
	int year;
	int month;
	int day;
};
DATE::DATE(int y, int m, int d) {
	year = y;
	month = m;
	day = d;
}
class People {
public:
	People(int num, SEX se, DATE birth, int iid);
	void getin(int num,SEX se ,DATE birth,int iid);
	void show();
	People(People &p1);
	~People() {};
private:
	int number;
	SEX sex;
	DATE birthday;
	int id;
};
People::People(int num, SEX se, DATE birth, int iid) {
	number = num;
	sex = se;
	birthday = birth;
	id = iid;
}
People::People(People& p1) {
	number = p1.number;
	sex=p1.sex;
    birthday=p1.birthday;
	id=p1.id;
}
void People::getin(int num, SEX se, DATE birth, int iid) {
	number = num;
	sex = se;
	birthday = birth;
	id = iid;
}
void People::show() {
	cout << "编号为：" << number<<endl;
	if (sex == girl) {
		cout << "性别为：女"<<endl;
	}
	else cout << "性别为：男"<<endl;
	cout << "出生日期为：";
	birthday.showdate();
	cout <<" id：" << id << endl;
	cout << "身份显示结束" << endl;
}
int main() {
	DATE d(2022, 3, 13);
	People man1(1, boy, d, 20220013);
	man1.show();
	DATE m(2011, 4, 2);
	man1.getin(2, girl, m, 20190023);
	People man2 = man1;
	man2.show();
}