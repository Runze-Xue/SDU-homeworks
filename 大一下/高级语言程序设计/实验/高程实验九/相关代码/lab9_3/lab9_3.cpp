#include<iostream>
#include"stack.h"

int main() {
	Stack<int> A;
	cout << "顺序插入3,4,5,6,7" << endl;
	A.insert(3);
	A.insert(4);
	A.insert(5);
	A.insert(6);
	A.insert(7);
	cout << "pop输出两次："<<endl;
	A.pop(); A.pop(); cout << endl;
	cout << "插入68" << endl;
	A.insert(68);
	cout << "pop输出四次："<<endl;
	A.pop(); A.pop(); A.pop(); A.pop(); cout << endl;
	cout << "插入35—40 6个数，并使用clear清空栈" << endl;
	for (int i = 35; i <= 40; i++) {
		A.insert(i);
	}
	A.clear();
	return 0;
}