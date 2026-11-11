#include<iostream>

class Node{
public:
	Node(int val):val(val)
	{
		next = nullptr;	
	};
	Node *next;
	~Node(){};
	int val; 
};

class Chain{
public:
	Chain(){
	firstNode = nullptr;
	listnum = 0;
	};
	Node* getfirstNode(){
		return firstNode;
	}
	void createChain(int n);
	void chaininsert();
	void chaindelete();
	void chaininverse();
	void chainfind();
	void exclusiveor(Node*first);//输出异或和 
	Node* begin();//返回firstNode 
	~Chain(){
		while(firstNode != nullptr){
			Node *temp = firstNode->next;
			delete firstNode;
			firstNode = temp;
		}
	}
	class iterator{
	public:
		iterator(Node* temp):temp(temp){};
		Node* temp;
		bool isend(){
			if(temp==nullptr) return 1;
			else return 0;
		} 
		iterator &operator++(int){
			this->temp = temp->next;
			return *this;
		}
		int operator*(){
			return temp->val;
		}
	};
	
	 
private:
	Node* firstNode;
	int listnum = 0 ;//记录结点个数		
};

void Chain::createChain(int n){
	listnum = n;
	Node* tempptr = nullptr;
	for(int i = 0; i < n; i++){
		int val;
		std::cin>>val;
		Node* temp =  new Node(val);
		if(i == 0){
			firstNode = temp;
			tempptr = temp;
		}
		else {	
		tempptr->next = temp;
		tempptr = tempptr->next;
		}
	}
}
void Chain::chaininsert(){
	listnum++;			//记录结点个数++ 
	int indexnum;		//输入idx 
	std::cin>>indexnum;
	int val;
	std::cin>>val; 
	Node* element = new Node(val);//new一个新结点 
	if(firstNode ==nullptr){//如果还没有结点 
		firstNode = element;
	}
	else if(indexnum == 0){					//如果插入结点处为链表第一个位置 
		element->next = firstNode;
		firstNode = element;
	}
	else {
		Node *leftptr = firstNode;
		for(int i = 0; i < indexnum - 1; i++){
			leftptr = leftptr->next;
		}
		element->next = leftptr->next;
		leftptr->next = element;
	} 
	return ;
}
void Chain::chaindelete(){
	listnum --;
	int val;
	std::cin>>val;
	Node* tempptr = firstNode;//记录遍历到的现在的结点 
	Node* lastptr = firstNode;//记录现在的结点的上一个结点 
	while(tempptr != nullptr){
		if(tempptr->val == val){
			lastptr->next = tempptr->next;
			tempptr->next = nullptr; 
			delete tempptr;
			
			return;
		}
		lastptr = tempptr;
		tempptr = tempptr->next;
	}
	std::cout<<"-1"<<'\n';
	return ;
}
void Chain::chaininverse(){
	Node *leftptr = nullptr;
	Node *cntNode = firstNode;
	if(cntNode == nullptr) return;
	while(cntNode!= nullptr){
		Node *tempright = cntNode->next;
		cntNode->next = leftptr;
		leftptr = cntNode;
		cntNode = tempright;
	}
	firstNode = leftptr;
}
void Chain::chainfind(){
	int val;
	std::cin>>val;
	Node* tempptr =  firstNode;
	int count = 0;
	while(tempptr!=nullptr){
		if(tempptr->val == val){
			std::cout<<count<<'\n';
			return ;
		}
		tempptr = tempptr->next;
		count++;
	}
	std::cout<<"-1"<<'\n';
}
void Chain::exclusiveor(Node*first){
	int sumor = 0; int countnum = 0;
	while(first != nullptr){
		sumor += countnum ^ first->val;
		countnum++;
		first = first->next;
	}
	std::cout<<sumor<<'\n';
}
Node* Chain::begin(){
	return firstNode;
}

int main(){
	int N, Q;
	std::cin>>N>>Q;
	Chain chain;
	chain.createChain(N);
	while(Q > 0){
		Q--;
		int sym;
		std::cin>>sym;
		if(sym ==1){
			chain.chaininsert();
		}
		else if(sym == 2){
			chain.chaindelete();
		}
		else if(sym == 3){
			chain.chaininverse();
		}
		else if(sym ==4){
			chain.chainfind();
		}
		else if(sym == 5){
			Chain::iterator it(chain.getfirstNode());
			int sumor = 0; int countnum = 0;
			int count = 0;
			for(; !it.isend(); it++)
			{
				sumor+=	countnum^ *(it);
				countnum++ ;
			}
			std::cout<<sumor<<'\n'; 
		}
		else{
			std::cout<<"wrong number"<<'\n';
			return 0;
		}
	}
	return 0;
} 
