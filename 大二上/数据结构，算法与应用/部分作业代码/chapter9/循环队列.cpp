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
class arrayQueue:public queue<T>{
private:
	int queueFront;
	int queueBack;
	int arrayLength;
	T *queue;
public:
	arrayQueue(int initialCapacity = 10){
		queue = new T[initialCapacity];
		queueFront = 0;
		queueBack = 0;
		arrayLength = initialCapacity;
	}
	~arrayQueue(){delete[]queue;}
	bool empty()const{return arrayLength ==0 ;}
	int size()const{return (arrayLength + queueBack - queueFront )%arrayLength ;}
	T&front(){return queue[(queueFront + 1)%arrayLength];}
	T&back(){return queue[queueBack];}
	void pop();
	T top();
	void push(const T &theElement);
	void enLarge();
};
template<typename T>
void arrayQueue<T>::pop(){
	if(queueFront == queueBack) std::cout<<"空，无法pop"<<'\n';//判断是否为空 
	else queueFront = (queueFront + 1)%arrayLength;
	return;
}
template<typename T>
T arrayQueue<T>::top(){
	if(queueFront == queueBack) std::cout<<"空，无法找到top"<<'\n';
	else return queue[(queueFront + 1)%arrayLength];
}
template<typename T>
void arrayQueue<T>::enLarge(){
	T* temp = new T[2*arrayLength];
	int start = (queueFront + 1)%arrayLength;
	if(start <= 1) copy(&queue[start], queue + start + arrayLength - 1, temp);
	else {
		cout<<"aaaaaaaaaaaaaa"<<'\n';
		copy(&queue[start], &queue[arrayLength], temp);
		copy(queue, &queue[queueBack+1], temp+arrayLength - start);
	}
	queueFront = 2*arrayLength - 1;
	queueBack = arrayLength - 2;
	arrayLength *= 2;
	delete[]queue;
	queue = temp;
}
template<typename T>
void arrayQueue<T>::push(const T &theElement){
	if((queueBack + 1)%arrayLength == queueFront) enLarge();
	queueBack = (queueBack + 1)%arrayLength;
	queue[queueBack] = theElement; 
}
int main(){
	arrayQueue<int> a(2);
	a.push(12);
	a.push(344);
	cout<<a.top()<<'\n';
	a.pop();
	cout<<a.top()<<'\n';
	return 0;
}








