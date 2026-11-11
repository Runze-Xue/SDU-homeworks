#include<iostream>

using namespace std;

template<typename T>
class queue{
public:
	virtual~queue(){}
	virtual bool empty()const = 0;
	virtual int size()const = 0;
	virtual T&front() = 0;
	virtual T&back() = 0;
	virtual void pop() = 0;
	virtual T top() = 0;
	virtual void push(const T &theElement) = 0;
};

template<typename T>
class chainNode{
public:
	chainNode* next = nullptr;
	T element;
};
template<typename T>
class LinkedQueue: public queue<T>{
private:
	chainNode<T>* queueFront =nullptr;
	chainNode<T>* queueBack = nullptr;
	int queueSize;
public:
	LinkedQueue(int initialCapacity = 10){
		queueSize = 0;
	};
	~LinkedQueue(){
		while(queueFront!= nullptr){
			chainNode<T>* temp = queueFront->next;
			delete queueFront;
			queueFront = temp;
		}
	};
	bool empty()const{return queueSize == 0;}
	int size()const{return queueSize;}
	T&front(){return queueFront->element;};
	T&back(){return queueBack->element;};
	void push(const T&theElement);
	T top(); 
	void pop(); 
};
template<typename T>
void LinkedQueue<T>::push(const T&theElement){
	chainNode<T>* temp = new chainNode<T>;
	temp->element = theElement;
	if(queueBack ==nullptr)queueBack = temp;
	else{
	queueBack->next = temp;
	queueBack = temp;} 
		if(queueFront == nullptr)queueFront = temp;
}
template<typename T>
T LinkedQueue<T>::top(){
	return queueFront->element;
}
template<typename T>
void LinkedQueue<T>::pop(){
	chainNode<T>* temp = queueFront->next;
	delete queueFront;
	queueFront = temp;
}

int main(){
	LinkedQueue<int> a;
	a.push(23);
	a.push(6666);
	a.push(777777);
	cout<<a.top();
	a.pop();
	cout<<a.top();
	a.pop();
	cout<<a.top();
	return 0 ;
}

