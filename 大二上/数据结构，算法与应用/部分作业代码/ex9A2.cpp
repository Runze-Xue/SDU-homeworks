#include<iostream>
struct Node{//树上节点 
	Node(){
	leftptr = nullptr;
	rightptr = nullptr;
	return;
	};
	Node(int v):value(v){
	leftptr = nullptr;
	rightptr = nullptr;
	height = 1;
	sumNode = 1;
	return ;
	};
	int value;//记录节点value 
	int height;//记录该节点高度 
	int sumNode;//记录以该节点为根的树的节点个数 
	Node* leftptr = nullptr;
	Node* rightptr = nullptr;
};


template<typename T>
class Stack{//栈类 
public:
	Stack(){
		element = new T[100000];
		listSize = 0 ;
		head = 0;
		tail = 0;
	}
	~Stack(){delete []element;}
	bool empty()const{return listSize == 0;}
	void push(T& a){
		tail++;
		element[tail] = a; 
		listSize++;
	}
	void pop(){
		if(!empty()){
			tail--;
			listSize--;
		}
	}
	T& top(){
		return element[tail];
	}
	void clear(){//清空栈 
		if(!this->empty()){
			this->pop();
		}
	}
	void OutStack(){
		while(!this->empty()){
			std::cout<<this->top();
			this->pop();	
		}
	}
private:
	T *element = nullptr;
	int listSize = 0;
	int head = 0;//头是栈的第一个元素索引，一般不进行操作 
	int tail = 0;//尾是数组尾,随着元素压入、弹出栈而不断改变 
};
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


class binaryTree{//二叉树类 
public:
	binaryTree(){
		int n = 0; std::cin>>n;
		Nodenum = n;
		root = new Node(1);
		h = new Node*[n + 1];//h是一个数组，实现O(1)复杂度寻找相应序号的节点值 
		h[1] = root;//将根节点赋给h[1] 
		int count = 1;//用于记录序号 
		int a, b;
		while(count <= n){//当没有输入完n组数据时持续循环 
			std::cin>>a>>b;//输入第n号节点的左右子节点值 
			Node* temp = h[count];//将相应第count号的节点从h数组中取出 
			if(a != -1){//如果插入的左节点不为空 
				Node* cntNode = new Node(a);
				temp->leftptr = cntNode;//插入左节点 
				h[a] = cntNode;//将新节点放入到h数组中相应位置 
			}
			if(b != -1){//如果插入的右结点不为空 
				Node* cntNode = new Node(b);
				temp->rightptr = cntNode;//插入右结点 
				h[b] = cntNode;//将新节点放入到h数组相应位置 
			}
			count++;
		}
		return;
	}
	void deleteOrder(Node* cnt){//用于析构函数删除new出的节点 
		if(cnt->leftptr == nullptr && cnt->rightptr == nullptr) delete cnt;
		if(cnt->leftptr != nullptr) deleteOrder(cnt->leftptr);
		if(cnt->rightptr != nullptr) deleteOrder(cnt->rightptr);
		delete cnt;
	}
	~binaryTree(){
		for(int i = 1; i <= Nodenum; i++){
			delete h[i];
		}
		delete h; 
	}
	void PreOrder(Node* cntptr){//前序遍历,顺便完成每个节点高度和作为根的子树节点总数的计算 
		if(cntptr == nullptr) return;
		std::cout<<cntptr->value<<' ';//输出该节点 
		PreOrder(cntptr->leftptr);//遍历左子树 
		PreOrder(cntptr->rightptr);//遍历右子树 
		int maxheight = 0;//对高度进行计算 ,高度等于该节点左右子树中最高的那一棵 + 1  
		if(cntptr->leftptr != nullptr) maxheight = (cntptr->leftptr)->height >maxheight?(cntptr->leftptr)->height : maxheight;
		if(cntptr->rightptr != nullptr) maxheight = (cntptr->rightptr)->height >maxheight?(cntptr->rightptr)->height : maxheight;
		cntptr->height = maxheight + 1;
		int maxNode = 1;//对总节点数进行计算，总节点数等于该节点左右子树中节点数之和 + 1 
		if(cntptr->leftptr != nullptr) maxNode += (cntptr->leftptr)->sumNode;
		if(cntptr->rightptr != nullptr) maxNode +=(cntptr->rightptr)->sumNode;
		cntptr->sumNode = maxNode; 
		return;
	}
	void preOrder(){
		PreOrder(root);
	}
	void inOrder(){//中序遍历 
		Stack<Node> s;//使用一个栈 
		s.push(*root);//将根节点入栈 
		while(!s.empty()){//当栈不为空时持续循环操作 
			Node temp = s.top();
			s.pop();
			if(temp.leftptr == nullptr && temp.rightptr == nullptr){//如果弹出元素的左孩子为空并且右孩子为空 
				std::cout<<temp.value<<' ';//输出该元素值 
				continue;
			}
			if(temp.rightptr != nullptr) s.push(*temp.rightptr);//如果该元素右孩子不为空,将右孩子压入栈 
			Node outtemp(temp.value);//outtemp用来输出，主动设置一个左右子树都为空，value值为当前节点的Node，并将其压入栈中 
			s.push(outtemp);//新结点入栈 
			if(temp.leftptr != nullptr) s.push(*temp.leftptr);//如果该元素左孩子不为空，将左孩子压入栈 
		}
	}
	void postOrder(){//后序遍历,与后序遍历栈操作的顺序区别在于，先将新构造的左右孩子都为空的节点压入栈,再将右孩子压入栈，最后将左孩子压入栈 
		Stack<Node> s;
		s.push(*root);
		while(!s.empty()){
			Node temp = s.top();
			s.pop();
			if(temp.leftptr == nullptr && temp.rightptr == nullptr){
				std::cout<<temp.value<<' ';
				continue;
			}
			Node outtemp(temp.value);//outtemp用来输出，左右子树都为空
			s.push(outtemp);
			if(temp.rightptr != nullptr) s.push(*temp.rightptr);
			if(temp.leftptr != nullptr) s.push(*temp.leftptr);
		}
	}
	void levelOrder(){//层次遍历 
		Queue<Node*> q(100000);
		q.push(root);
		while(!q.empty()){//队列不为空时持续操作 
			Node* temp = q.top();//取出队首元素 
			q.pop();
			if(temp->leftptr != nullptr) q.push(temp->leftptr);//如果该元素左孩子不为空，将左孩子入队 
			if(temp->rightptr != nullptr) q.push(temp->rightptr);//如果该元素右孩子不为空，将右孩子入队 
			std::cout<<temp->value<<' ';//输出该元素值 
		}
	}
	void Outheight(){//输出高度 
		for(int i = 1; i <= Nodenum; i++){
			Node* temp = h[i];
			std::cout<<temp->height<<' ';
		}
	}
	void OutsumNode(){//输出节点总数 
		for(int i = 1; i <= Nodenum; i++){
			Node* temp = h[i];
			std::cout<<temp->sumNode<<' ';
		}
	}
private:
	Node* root = nullptr;//根节点 
	Node** h;//一个数组，直接映射树中元素 
	int Nodenum = 0;//树的总节点个数 
};
int main(){
	binaryTree tree;
	tree.preOrder(); std::cout<<'\n';
	tree.inOrder(); std::cout<<'\n';
	tree.postOrder(); std::cout<<'\n';
	tree.levelOrder(); std::cout<<'\n';
	tree.OutsumNode();std::cout<<'\n';
	tree.Outheight();std::cout<<'\n';
	return 0;
}
