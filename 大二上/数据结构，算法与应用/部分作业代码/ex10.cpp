#include<iostream>

template<typename T>
class MinHeap{//最小堆类 
public:
	MinHeap(){//最小堆的初始化函数，完成堆的初始化操作 
		int n;
		std::cin>>n;
		heap = new T[6000];//因为第一个root点在 1 处 
		for(int i = 1; i <= n; i++){
			std::cin>>heap[i];//先记录到数组中 
		}
		heapSize = n;
		for(int root = heapSize / 2; root >= 1; root --){//最后一个点是 heapSize, 最后一个含有孩子的节点是heapSize / 2 
			T rootElement = heap[root];//记录该节点元素值 
			int child = 2 * root;//左孩子索引
			while(child <= heapSize){
				if(child < heapSize && heap[child] > heap[child + 1]) child ++;//右孩子存在且右孩子值小于左孩子值，则child++ 
				if(rootElement <= heap[child]) break;//如果根节点 < = 最大子节点，则break;
				heap[child / 2] = heap[child];
				child = child * 2;
			}
			//当结束时,heap[child] > rootElement, 此时heap[child / 2]已经上移， 所以heap[child / 2]空出，令heap[child / 2] = rootElement;  
			heap[child/2] = rootElement;
 		}
	}
	~MinHeap(){
		delete heap;
	}
	void push(T cnt){//插入操作 
		heapSize++;
		int cntNodeloc = heapSize;
		while(cntNodeloc!=1 && heap[cntNodeloc / 2] > cnt){//没有到堆顶且其父节点值比cnt大，则父节点下沉, cntNodeloc更新为父节点索引 
			heap[cntNodeloc] = heap[cntNodeloc / 2];
			cntNodeloc = cntNodeloc / 2;
		}
		heap[cntNodeloc] = cnt;
	}
	void pop(){//弹出删除操作 
		T lastElement =  heap[heapSize];
		heapSize --;
		int cntNodeloc = 1; //记录父索引 ,
		int child = 2;//child存储较大的孩子的索引 
		while(child <= heapSize){//child索引没有到堆底时 
			if(child  < heapSize && heap[child] > heap[child + 1]) child++;//找到左右孩子中较小者 
			if(lastElement <= heap[child]) break;
			heap[cntNodeloc] = heap[child];
			cntNodeloc = child;
			child = 2 * child;
		}
		heap[cntNodeloc] = lastElement;
	}
	void Outheap(){
		for(int i = 1; i <= heapSize; i++){
			std::cout<<heap[i]<<' ';
		}
	}
	void heapOrder(){
		while(heapSize > 0 ){
			std::cout<<this->top()<<' ';
			this->pop();
		}
	}
	T& top(){
		return heap[1];
	}
	
		
private:
	T *heap;
	int heapSize;	
};
int main(){
	MinHeap<int> heap;
	std::cout<<heap.top()<<'\n';
	int m; std::cin>>m;
	for(int i = 1; i <= m; i++){
		int sym; std::cin>>sym;
		if(sym == 1){
			int num; std::cin>>num;
			heap.push(num);
			std::cout<<heap.top()<<'\n';
		}
		else if(sym == 2){
			heap.pop();
			std::cout<<heap.top()<<'\n';
		}
		else if(sym == 3){
			MinHeap<int> Orderheap;
			Orderheap.heapOrder();
		}
	}
	return 0 ;
}
