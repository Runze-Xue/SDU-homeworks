#ifndef STACK_H_
#define STACK_H_

#include"link.h"
#include<iostream>

using namespace std;

template<typename T>
class STACK{
private:
	LinkedList<T> Stack;
public:
	STACK() :Stack() {};
	void insert(T m);
	T pop();
	void clear();
};
template<typename T>
void STACK<T>::insert(T m) {
	Stack.insertFront(m);
}
template<typename T>
T STACK<T>::pop() {
	T kep = Stack.data();
	Stack.next();
	return kep;
}
template<typename T>
void STACK<T>::clear() {
	while (!Stack.endOfList()) {
		pop();
	}
	cout << "ÒÑÇå¿ÕÕ»" << endl;
}





















#endif //STACK_H_