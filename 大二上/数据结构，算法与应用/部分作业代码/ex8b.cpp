#include<iostream>
struct Node{
	Node * next;//连接下一个节点的指针 
	int k;//key值 
};

class hash{
public:
	hash(int D){
		arrayLength = D;
		element = new Node* [D];//element是Node*类型的数组 
		for(int i = 0; i < D; i++){
			element[i] = nullptr;//把数组每一个指针元素置为空指针 
		}
	}
	~hash(){
		for(int i = 0; i < arrayLength; i++){
			Node* ptr = element[i];
			while(ptr != nullptr){//将所有的new出的Node节点全部delete 
				Node* temp = ptr->next;
				delete ptr;
				ptr = temp;
			}
		}
		delete []element;
	}
	void insert(int x){
		int mapnum = x % arrayLength; 
		if(element[mapnum] == nullptr){
			element[mapnum] = new Node;
			element[mapnum]->k = x;
			element[mapnum]->next = nullptr;
			return;
		}
		Node* cntptr = element[mapnum];//记录遍历到的节点 
		Node* leftptr = nullptr;//记录上一个节点 
		while(cntptr != nullptr){
			if(cntptr->k == x){
				std::cout<<"Existed"<<'\n';
				return;
			}
			leftptr = cntptr;
			cntptr = cntptr->next;
		}//直到整个cnt到链表末尾结束
		//没有找到x，则在链表末尾插入x 
		Node* temp = new Node;
		temp->k = x;
		temp->next = nullptr;
		leftptr->next = temp;
		return;//
	}
	void search(int x){
		int mapnum = x % arrayLength;
		if(element[mapnum] == nullptr){//如果链表中第一个元素为空，则不存在x元素 
			std::cout<<"Not Found"<<'\n';
			return;
		}
		int count = 0;
		Node* tempptr = element[mapnum];
		int sym = 0;//探查是否找到 
		while(tempptr != nullptr){
			count++;
			if(tempptr->k == x){
				sym = 1;
			}
			tempptr = tempptr->next;
		}
		if(sym == 1)std::cout<<count<<'\n';
		else std::cout<<"Not Found"<<'\n';
		return;
	}
	void hashdelete(int x){
		int mapnum = x % arrayLength;
		if(element[mapnum] == nullptr){//如果链表第一个元素为空，则不存在x元素 
			std::cout<<"Delete Failed"<<'\n';
			return;
		}
		int count = 0;//记录链表删除x后的长度
		if(element[mapnum]->k == x){//如果要删除的节点是头结点 
			Node* temp = element[mapnum]->next;
			delete element[mapnum];
			element[mapnum] = temp;
		}
		Node* cntptr = element[mapnum];//记录遍历到的现在的元素指针 
		Node* leftptr = nullptr;//记录上一个元素的指针 
		while(cntptr != nullptr){//当没有遍历到链表尾部 
			if(cntptr->k == x){//如果找到该删除元素，则将其删除 
				Node* temp = cntptr->next;
				delete cntptr;
				leftptr->next = temp;
				cntptr = temp;
			}
			else{			//如果不是删除元素，则将count++，并将两个指针移动到下一个 
				count++;
				leftptr = cntptr;
				cntptr = cntptr->next;
			}
		} 
		std::cout<<count<<'\n';
		return;
	}
	void Outhash(){//用于输出整个哈希表 
		for(int i = 0; i<arrayLength; i++){
			Node* temp = element[i];
			while(temp != nullptr){
				std::cout<<temp->k<<' ';
				temp = temp->next;
			}
			std::cout<<'\n';
		}
		return;
	}
		
		 
private:
	Node **element;//定义一个指针数组  
	int arrayLength;//存储D的大小 
};
int main(){
	int D, m;
	std::cin>>D>>m;
	hash h(D);
	for(int i = 1; i <= m; i++){
		int opt, x;
		std::cin>>opt>>x;
		if(opt == 0){
			h.insert(x);
		}
		else if(opt == 1){
			h.search(x);
		}
		else if(opt == 2){
			h.hashdelete(x);
		}
	}
	return 0;
}

