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
protected:
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
	bool empty()const{return queueFront ==queueBack ;}
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



//
//void changearrayLength(){
//T*newQueue = new T[2*arrayLength];
//int start = (theFront)
//5 1）i 
//template<class T>
//class queue{
////以下是新增加的方法 
//public:
//	virtual void PutQueue();//输入一个队列
//	virtual void OutQueue();//输出一个队列
//	virtual void SeperateQueue();//将一个队列分为两个新队列 
//	virtual void MergeQueue();//实际上要传入两个队列参数，在派生类中将会对MergeQueue()实现并重载 
//};

//template<class T>
//class extendedQueue:public queue<T>{
//public:
//	virtual ~extendedQueue();//添加其析构函数 
//  int arrayList;
//}; 
template<class T>
class extendedarrayQueue: public arrayQueue<T>{//别忘了继承extendedQueue,只是因为这里没写extendedQueue:   , public extendedQueue<T> 
public:
	void enLargeArrayList(){
		return ;
	}
	~extendedarrayQueue(){}
	void PutQueue(){
		int n; std::cin>>n; //n 为要输入队列的个数
		while(n + this->size() > arrayLength - 1){//如果队列中已有元素个数和输入队列的元素个数之和比数组长度-1要大，那么扩大数组长度 
			this->enLargeArrayList();//见课本P211,程序9-3, 为该程序命名为enLargeArray 
		}
		T temp;				//记录队列中每个元素值 
		for(int i = 1; i <= n; i++){
			cin>> temp;			//从队首到队尾，依次输入队列元素
			this->push(temp); 
		}
	}
	void OutQueue(){
		while(!this->empty()){
			T temp = this->top();
			this->pop();
			cout<<temp<<' ';
		}
	}
	void SeperateQueue(){
		extendedarrayQueue<T>* q1 = new extendedarrayQueue<T>;//q1记录奇数项元素 
		extendedarrayQueue<T>* q2 = new extendedarrayQueue<T>;//q2记录偶数项元素 
		while(!this->empty()){
			if(!this->empty()){
				T temp = this->top();
				q1->push(temp);
				this->pop();
			}
			if(!this->empty()){
				T temp = this->top();
				q2->push(temp);
				this->pop();
			}
		}
//		q1->OutQueue();
//		q2->OutQueue();
//		delete q1;delete q2;
	}
	void MergeQueue(){} //实现抽象类中定义 
	void MergeQueue(extendedarrayQueue& A, extendedarrayQueue& B){//由一个空队列来调用该函数，最后合并结果在该空队列里,从队列A开始 
		while(A.size() + B.size() > arrayLength - 1) this->enLargeArrayList();//见课本P211页程序9-3,数组队列长度加倍 
		while(!A.empty() || !B.empty()){
			int temp;
			if(!A.empty()){
				temp = A.top();
				A.pop();
				this->push(temp);
			}
			if(!B.empty()){
				temp = B.top();
				B.pop();
				this->push(temp);
			} 
		}
	}
private:
	int arrayLength;
};




int main(){
	extendedarrayQueue <int> queue1;
	extendedarrayQueue <int> queue2;	
	extendedarrayQueue <int> queue3;
	queue1.PutQueue();
	queue2.PutQueue();
//	queue3.MergeQueue(queue1, queue2);
	queue3.OutQueue();
	return 0;
}








