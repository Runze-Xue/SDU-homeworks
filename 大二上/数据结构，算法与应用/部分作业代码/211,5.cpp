5 
1），2）两问 
template<class T>
class extendedQueue:public queue<T>{
public:
	virtual ~extendedQueue();//添加其析构函数 
	virtual void PutQueue();//输入一个队列
	virtual void OutQueue();//输出一个队列
	virtual void SeperateQueue();//将一个队列分为两个新队列 
	virtual void MergeQueue();//实际上要传入两个队列参数，在派生类中将会对MergeQueue()实现并重载 
}; 
3） 
template<class T>
class extendedarrayQueue: public arrayQueue<T>, public extendedQueue<T>{//别忘了继承extendedQueue,只是因为这里没写extendedQueue:   , public extendedQueue<T> 
public:
	void enLargeArrayList(){
		return ;
	}
	~extendedarrayQueue(){}
	void PutQueue(){
		int n; std::cin>>n; //n 为要输入队列的个数
		while(n + this->size() > arrayLength - 1){//如果队列中已有元素个数和输入队列的元素个数之和比数组长度-1要大，那么扩大数组长度 
			this->enLargeArrayList();//见课本P211,程序9-3, 主动为该程序命名为enLargeArray 
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
			cout<<temp<<' ';	//输出 
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
		delete q1; delete q2;//如果q1 q2不再使用，则delete 
	}
	void MergeQueue(){} //实现抽象类中定义 
	void MergeQueue(extendedarrayQueue& A, extendedarrayQueue& B){//由一个空队列来调用该函数，最后合并结果在该空队列里,从队列A开始 
		while(A.size() + B.size() > arrayLength - 1) this->enLargeArrayList();//见课本P211页程序9-3,数组队列长度加倍 
		while(!A.empty() || !B.empty()){//A,B中有任意一个不空，循环继续 
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
	T*element;
 	int arrayLength;
 	int listSize;
};
