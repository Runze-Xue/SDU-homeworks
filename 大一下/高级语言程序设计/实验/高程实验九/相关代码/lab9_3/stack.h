//LinkedList.h
#ifndef STACK_H
#define STACK_H
#include"LinkedList.h"

using namespace std;

template<typename T>
class Stack {
private:
	LinkedList<T> LINK;
public:
	Stack() {};
	void pop();
	void insert(T m);
	void clear();
};

template<typename T>
void Stack<T>::insert(T m) {
	LINK.insertFront(m);
}
template<typename T>

void Stack<T>::pop() {
	LINK.reset(0);
	cout << LINK.data()<<"  ";
	LINK.deleteCurrent();
}
template<typename T>
void Stack<T>::clear() {
	while (!LINK.isEmpty()) {
		LINK.reset(0);
		cout << LINK.data()<<"  ";
		LINK.deleteCurrent();
	}
}












































#endif  //STACK_H