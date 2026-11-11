#include<iostream>

using namespace std;

class BaseClass {
public:
	BaseClass()
	{
		cout << "执行BaseClass类的构造函数"  << endl;
	};
	~BaseClass() {
		cout << "执行BaseClass类的析构函数"  <<endl;
	};
	void getNumber();
protected:
	static int number;
};
int BaseClass::number = 0;

class DerivedClass :public BaseClass {
public:
	DerivedClass():BaseClass() {
		number++;
		cout << "执行DerivedClass类的构造函数,当前对象数量：" << number << endl;
	};
	~DerivedClass() {
		number--;
		cout << "执行DerivedClass类的析构函数,当前剩余对象数量" <<number<< endl;
	};
};
void BaseClass::getNumber() {
	cout << number << endl;
}
int main() {
	DerivedClass dv1;
	DerivedClass dv2;
	DerivedClass dv3;
	return 0;
}