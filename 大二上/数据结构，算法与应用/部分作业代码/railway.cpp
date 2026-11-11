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
	arrayQueue<int> rail[2];

	for(int i =0; i <= 2; i++){
		int temp;
		cin>>temp;
		if(rail[0].empty()&&rail[1].empty()){//两个表都为空
			rail[0].push(temp); 
		}
		
		else if(!rail[0].empty()&&rail[1].empty()){
			if(temp < rail[0].top()) rail[1].push(temp);
			else rail[0].push(temp);
		}
		else{
			int num = 0; int max = 0;
			for(int i = 0; i <= 1; i++){
				if(max < rail[i].top()){
					max = rail[i].top();
					num = i;
				} 
			}
			rail[num].push(temp);
		}
	}
//	while(!rail[0].empty() || !rail[1].empty()){
//		int num = 0; int min = 0x3fff;
//		for(int i = 0; i <= 1; i++){
//			if(!rail[i].empty()&&rail[i].top() < min){
//				num =  i; min = rail[i].top();
//			}
//		}
//		std::cout<<min<<' ';
//		rail[num].pop(); 
//	}
	while(!rail[1].empty()){
		cout<<rail[1].top();
		rail[1].pop();
	}
	return 0;
}








