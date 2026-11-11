#include<iostream>
struct Node{
	int key = 0;
	Node* leftptr = nullptr;
	Node* rightptr = nullptr;

};
class MinHeap{//使用最小堆 
public:
	MinHeap(){
	heapSize = 0;
	heap = new Node*[100];
	for(int i = 0; i < 100; i++){
		heap[i] = nullptr;
	}
	}	
	~MinHeap(){//析构函数 
		for(int i = 0; i < 100; i++){
			delete heap[i];
		}
		delete []heap;
	}
	void push(Node* cnt){
		heapSize++;
		int cntNodeloc = heapSize;
		while(cntNodeloc!=1 && heap[cntNodeloc / 2]->key > cnt->key){
			heap[cntNodeloc] = heap[cntNodeloc / 2];
			cntNodeloc = cntNodeloc / 2;
		}
		heap[cntNodeloc] = cnt;
	}
	void pop(){
		Node* lastElement =  heap[heapSize];
		heapSize --;
		int cntNodeloc = 1; 
		int child = 2;// 
		while(child <= heapSize){
			if(child  < heapSize && heap[child]->key > heap[child + 1]->key) child++;
			if(lastElement->key <= heap[child]->key) break;
			heap[cntNodeloc] = heap[child];
			cntNodeloc = child;
			child = 2 * child;
		}
		heap[cntNodeloc] = lastElement;
	}
	void Outheap(){
		for(int i = 1; i <= heapSize; i++){
			std::cout<<heap[i]->key<<' ';
		}
	}
	void heapOrder(){
		while(heapSize > 0 ){
			std::cout<<this->top()<<' ';
			this->pop();
		}
	}
	Node* & top(){
		return heap[1];
	}
	
		
private:
	Node **heap;//使用一个二维数组，能够让最小堆能够存储树结构 
	int heapSize;
};

class HuffmanTree{
public:
	HuffmanTree(){
		heap = new MinHeap ;
		char c = getchar();
		element = new int[26];
		for(int i = 0; i < 26; i++){
			element[i] = 0;
		}
		while(c != '\n'){//输入字符串 
			element[c-'a']++;
			c = getchar();
		}
		for(int i = 0; i < 26; i++){//初始把含有一个元素的树压入堆中 
			if(element[i] == 0) continue;
			Node* temp = new Node;
			temp->key = element[i];
			temp->leftptr = nullptr;
			temp->rightptr = nullptr;
			heap->push(temp);
		}
		
	}
	~HuffmanTree(){
		delete heap;
	}
	
	void merge(){//合并两个值最小的树 
		Node* a,*b;
		for(int i = 1; i <= 25; i++){
			if(element[i] == 0) continue;
			//取出两个值最小的树并弹出 
			a = heap->top();
			heap->pop();
			b = heap->top();
			heap->pop();
			Node* temp = new Node;
			//生成新树 
			temp->key = a->key + b->key;
			temp->leftptr =  a;
			temp->rightptr = b;
			heap->push(temp);	
		}
	}
	int  outans(Node* temp, int height = 0){//霍夫曼编码长度 = height * 频次  
		if(temp->leftptr==nullptr && temp->rightptr == nullptr) return temp->key * height;
	    else if(temp->leftptr != nullptr && temp->rightptr != nullptr) return outans(temp->leftptr, height + 1) + outans(temp->rightptr, height + 1);
		else if(temp->leftptr != nullptr && temp->rightptr == nullptr) return outans(temp->leftptr, height + 1);
		else if(temp->leftptr == nullptr && temp->rightptr != nullptr) return outans(temp->rightptr, height + 1);
	}
	void Outans(){
		int ans = outans(heap->top(), 0);
		std::cout<<ans;
	}
	void Outtree(Node* temp){
		std::cout<<temp->key<<'\n';
	}
private:
	MinHeap* heap;
	int * element;
};

int main(){
	HuffmanTree tree;
	tree.merge();//将每个合并 
	tree.Outans();//输出霍夫曼编码长度 
	return 0 ;
}
