#include<iostream>
#include"stack.h"

using namespace std;

int main() {
	STACK<int> test;
	test.insert(4);
	test.insert(5);
	test.insert(6);
	test.insert(43);
	test.insert(938);
	//cout << "顺序输入4,5,6,43,938 输出如下：" << endl;
	//cout << test.pop()<< endl;
	//cout << test.pop() << endl;
	//cout << test.pop() << endl;
	test.insert(34);
	cout << test.pop() << endl;
	cout << test.pop() << endl;
	return 0;

}