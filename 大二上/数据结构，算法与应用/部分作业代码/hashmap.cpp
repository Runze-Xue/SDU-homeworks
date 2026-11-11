#include<iostream>

struct Node{
	Node(){
	leftptr = nullptr;
	rightptr = nullptr;
	};
	Node(int v):value(v){
	leftptr = nullptr;
	rightptr = nullptr;
	deleteptr = nullptr;//当独立链表被删除时使用 
	height = 1;
	sumNode = 1;
	};
	int value;
	int height;
	int sumNode;
	Node* leftptr = nullptr;
	Node* rightptr = nullptr;
	Node* deleteptr = nullptr;
};
//哈希表
struct pair{
	pair(){
	}
	pair(int key, Node* ptr){
		k = key; nodePtr = ptr;
	}
	int k;
	Node* nodePtr = nullptr;
}; 

class hash{
public:
	hash(int D){
		element = new pair[D];
		sym = new bool[D];
		for(int i = 0; i<D; i++){
			sym[i] = 0;
		}
		arrayLength = D;
	}
	~hash(){
		delete []element;
		delete []sym;
	}
	void insert(pair x){
		int mapnum = x.k % arrayLength;
		int countnum = 0;//判断是否循环过一遍 
		while(countnum < arrayLength && sym[mapnum] == 1){
			if(element[mapnum].k == x.k){
				std::cout<<"Existed"<<'\n';
				return ;
			}
			countnum++;
			mapnum++; mapnum = mapnum % arrayLength;
		}//直到循环过一遍或者找到了一个空位停止 
		if(sym[mapnum] == 0){
			element[mapnum] = x;
			sym[mapnum] = 1;
		}
		return;
	}
	Node* search(int x){//x实际为k值 
		int mapnum = x % arrayLength;
		int countnum = 0;//用来判断循环是否循环过一遍 
		while(countnum < arrayLength){
			if(sym[mapnum] == 0){
				std::cout<<"-1"<<'\n';
			}
			if(element[mapnum].k == x){
				std::cout<<mapnum<<'\n';
				return element[mapnum].nodePtr;
			}
			countnum++; mapnum++;
			mapnum = mapnum % arrayLength; 
		}
		std::cout<<"-1"<<'\n';//遍历了整个数组都没有找到
	}

private:
	pair* element;//用来记录表 
	bool * sym;//用来记录桶是否空,0 为空， 1为占用
	int arrayLength;//记录哈希表的表长度,用来取模值 
};
int main(){

	return 0;
}
