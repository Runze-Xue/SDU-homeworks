#include<iostream>

struct Node{
	Node(){};
	Node(int v){
		value = v;
		leftptr = nullptr;
		rightptr = nullptr;
	}
	int value;
	Node* leftptr = nullptr;
	Node* rightptr = nullptr;
};
class binaryTree{
public:
	//总结两句结论1 上一次与本次的相对位置决定是左右节点中的哪个
	// 2 跟随方向，加入到最近的空点中 
	binaryTree(){
		int n = 0;
		std::cin>>n;
		num = n;
		preOrder = new int[n + 1];
		inOrder = new Node*[n + 1];
		sym = new bool[n + 1];
		Orderin = new int[n + 1];
		for(int i = 1; i <= n; i++){
			std::cin>>preOrder[i];
		}
		for(int i = 1; i <= n; i++){
			int value ; std::cin>>value;
			inOrder[i] = new Node(value);
			Orderin[value] = i;//该数组索引为值，值为inOrder索引 
			sym[i] = 0;//代表未入树 
			
		}
		root = inOrder[Orderin[1]];
	}
	~binaryTree(){
	    delete []preOrder;
    	delete []sym;
    	delete Orderin;//记录相应中序遍历元素的相应位置 
		for(int i = 1; i <= num; i++){
			delete inOrder[i];
		}
		delete []inOrder;
	}
	void buildTree(){
		//Orderin 记录相应的值在inOrder中的索引，意义为索引
		//inOrder为一个Node* 矩阵，保存着所有的节点
		//sym 判断对应索引位置是否入树
		//preOrder是int类型的前向遍历的结果矩阵 
		sym[Orderin[1]] = 1;
		int last;//记录上一次操作的节点在inOrder中的索引
		for(int i = 2; i <= num; i++){
			int last = Orderin[preOrder[i - 1]];//last是上一次遍历元素的位置 
			int temp = Orderin[preOrder[i]];//temp是现在遍历到的元素在inOrder中的位置 
			if(temp < last){//插入左节点 
				sym[temp] = 1;
				int move = temp + 1;
				while(sym[move] == 0 || (sym[move] == 1 && inOrder[move]->leftptr!= nullptr)) move++;//如果遍历到的点没有出现在树中，或者该点左孩子指针不为空则移动，直到找到第一个出现在树中且左孩子指针为空的点 
				//当结束遍历时，说明找到了相应点 
				inOrder[move]->leftptr = inOrder[temp];//连接 
			}	
			else if(temp > last){//插入右节点 
				sym[temp] = 1;
				int move = temp - 1;
				while(sym[move] == 0 || (sym[move] == 1 && inOrder[move]->rightptr!= nullptr)) move--;//如果遍历到的点没有出现在树中，或者该点右孩子指针不为空则移动，直到找到第一个出现在树中且右孩子指针为空的点 
				inOrder[move]->rightptr = inOrder[temp];//连接 
			}
		}
	}
	void PostOrder(){
		this->postOrder(root);
	}
	void postOrder(Node* cnt){//递归实现后序遍历 
		if(cnt == nullptr) return;
		if(cnt->leftptr != nullptr) {
		postOrder(cnt->leftptr);}
		if(cnt->rightptr != nullptr) {
		postOrder(cnt->rightptr);}
		std::cout<<cnt->value<<' ';
	}
	
private:
    int *preOrder;
    Node **inOrder;
    bool * sym;//用于判断相应位置元素是否在树中,0为空，1为存在 
    int *Orderin;//记录相应中序遍历元素的相应位置 
    Node* root;
    int num = 0;
};
int main(){
	binaryTree tree;
	tree.buildTree();
	tree.PostOrder();
	return 0 ;
}
