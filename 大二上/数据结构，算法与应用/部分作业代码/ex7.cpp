#include<iostream>

template<typename T>
class Queue{//循环队列，实现push，pop，top等功能，先入先出 
public:
	Queue(int n){//构造函数 
		qBack = 0;
		qFront = 0;
		arrayLength = n + 1;
		element = new T[n + 1];//申请n+1个空间，因为循环队列数组中要空一个 
	}
	~Queue(){
		delete []element;
	}
	int Size(){//判断队列中元素数量
		return  (arrayLength + qBack - qFront)%arrayLength;//模arrayLength保证qBack在qFront右方时计算结果不超出arrayLength， 
	}
	bool empty(){//判断队列是否为空 
		return qBack == qFront;//当qBack = qFront 时为空，(qBack + 1)%arrayLength = qFront时为满 
	}
	void push(T a){
		if((qBack + 1)%arrayLength == qFront){//判断队列是否满 
			std::cout<<"FULL QUEUE"<<'\n';
			return;
		}
		qBack = (qBack + 1)%arrayLength;
		element[qBack] = a;
	}
	T top(){
		if(this->empty()) std::cout<<"empty"<<'\n'; 
		return element[(qFront + 1)%arrayLength];
	}
	void pop(){
		qFront = (qFront + 1)%arrayLength;//弹出操作，即将qFront向前移动1，模arrayLength确保qFront不超过 
	}
private:
	T *element = nullptr;//元素数组指针 
	int qBack;//记录队尾,随着元素入队而移动 
	int qFront;//记录队首，随着元素出队而移动
	int arrayLength;//记录数组大小 
};
int main(){
	int n;
	std::cin>>n;
	Queue<int>q(n);
	for(int i = 1; i<=n; i++){
		q.push(i);
	}
	while(q.Size()>1){
		q.pop();
		q.push(q.top());
		q.pop();
	}
	std::cout<<q.top();
	return 0;
}
