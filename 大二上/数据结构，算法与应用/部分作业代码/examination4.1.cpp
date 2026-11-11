#include<iostream>

class Node{	    //Node作为链表结点 
public:
	Node(int val):val(val)
	{
		next = nullptr;	
	};
	Node *next;
	~Node(){};
	int val; 
};

class Chain{	//链表 
public:
	Chain(){
	firstNode = nullptr;
	listnum = 0;
	};
	Node* getfirstNode(){
		return firstNode;
	}
	void createChain(int n);//实现开始时生成一个链表 
	void chaininsert();		//实现链表插入操作 
	void chaindelete();		//实现链表删除操作 
	void chaininverse();	//实现链表逆置操作 
	void chainfind();		//实现链表查询操作 
	void exclusiveor(Node*first);//输出异或和 
	Node* begin();//返回firstNode 
	~Chain(){
		while(firstNode != nullptr){
			Node *temp = firstNode->next;
			delete firstNode;
			firstNode = temp;
		}
	}
	class iterator{			//迭代器类，重载++，*等运算符，实现指针功能 
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
		if(i == 0){				//如果链表中还没有结点，直接把该结点作为头结点 
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
	std::cin>>val;			//输入要删除的值 
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
	Node *leftptr = nullptr;	//记录操作结点的左节点 
	Node *cntNode = firstNode;	//记录操作结点 
	if(cntNode == nullptr) return;//如果该操作结点已经是空，那么说明已经完成了逆置 
	while(cntNode!= nullptr){	   
		Node *tempright = cntNode->next;	//记录操作结点的右结点 
		cntNode->next = leftptr;			//将现在结点指向记录的左节点 
		leftptr = cntNode;					//将左节点跳转到现在结点 
		cntNode = tempright;				//将现在结点跳转到右结点 
	}
	firstNode = leftptr;					//因为cntNode == nullptr, 所以leftptr是最后的结点，将头指针单独指向leftptr完成逆置 
}
void Chain::chainfind(){
	int val;
	std::cin>>val;
	Node* tempptr =  firstNode;
	int count = 0;
	while(tempptr!=nullptr){
		if(tempptr->val == val){
			std::cout<<count<<'\n';			//找到则输出索引 
			return ;
		}
		tempptr = tempptr->next;
		count++;
	}
	std::cout<<"-1"<<'\n';
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
			Chain::iterator it(chain.getfirstNode());	//使用迭代器，并获得要遍历链表的头指针 
			int sumor = 0; int countnum = 0;			
			int count = 0;	
			for(; !it.isend(); it++)					//使用迭代器进行遍历 
			{
				sumor+=	countnum^ *(it);				//计算索引与元素异或和 
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
