#include<iostream>
#include"node.h"
#include"link.h"

using namespace std;
int main() {
	LinkedList<int> A;
	LinkedList<int> B;
	int g = 0;
	cout << "输入A的5个结点值：" << endl;
	while (g < 5) {
		int number;	 cin >> number;
		A.insertRear(number);
		g++;
	}
	g = 0;
	cout << "输入B的5个结点值：" << endl;
	while(g<5){
		int number = 0; cin >> number;
		B.insertRear(number);
		g++;
	}
	A.reset(0); B.reset(0);
	cout << "A的结点值为：";
	g = 0;
	while (g<5) {
		cout << A.data()<<"  ";
		if(g!=5)A.next();
		g++;
	}
	cout << "B的结点值为：" << endl;
	g = 0;
	while (g < 5) {
		cout << B.data()<<"  ";
		if(g!=5)B.next();
		g++;
	}
	cout << "将B中元素连接到A尾部：" << endl;
	A.reset(A.getSize());
	g = 0;
	B.reset(0);
	while (g < 5) {
		A.insertRear(B.data());
		B.next();
		g++;
	}
	g = 0;
	A.reset(0);
	while (g < 10) {
		cout << A.data() << " ";
		A.next();
		g++;
	}
	return 0;
}