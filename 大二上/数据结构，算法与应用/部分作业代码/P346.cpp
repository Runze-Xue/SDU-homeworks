#include<iostream>

using namespace std;

struct Node{
	int value;//节点值 
	Node* leftptr = nullptr;//节点左子树根节点 
	Node* rightptr = nullptr;//节点右子树根节点 
};
class searchTree{//搜索树类 
public:
	searchTree(int n){//构造函数，输入元素节点个数生成一颗树 
	for(int i = 0; i < n; i++){
		int v; cin>>v;
		Node* cnt = head;
		Node* temp = nullptr;
		if(head == nullptr){//如果head为空，则将第一个元素赋给head 
			head = new Node;
			head->value = v;
		}
		else{
			while(cnt != nullptr){//通过与子树根节点比对大小，最终找到一个空节点 
				if(cnt->value >v){
				temp = cnt;
				cnt = cnt->leftptr;
				}
				else if(cnt->value < v) {
				temp = cnt;
				cnt = cnt->rightptr;}
			}
			Node* newNode = new Node;//new一个Node值，v值赋给Node后插入找到的空节点 
			newNode->value = v;
			newNode->leftptr = nullptr;
			newNode->rightptr = nullptr;
			if(temp->value < v){
				temp->rightptr = newNode;
			}
			else if(temp->value > v){
				temp->leftptr = newNode;
			}
		}
	}
};
	static int count ;
	Node* gethead(){//获得head指针 
		return head;
	}
	void inorder(int a[], Node* cnt){//中序遍历，将树中元素排列到a数组中 
		if(cnt == nullptr)return;
		inorder(a,cnt->leftptr);
		a[count] = cnt->value;
		count++;
		inorder(a,cnt->rightptr);
	}
	void deletetree(Node* cnt){
		if(cnt == nullptr) return;
		deletetree(cnt->leftptr);
		deletetree(cnt->rightptr);
		delete cnt;//遍历完左右子树后删除该节点 
	}
	~searchTree(){
		deletetree(head);//调用函数deletetree删除树 
	}

private:
	Node* head = nullptr;
};
int searchTree::count = 0;

int main(){
	int n; cin>>n;
	searchTree s(n);
	int a[1000];
	s.inorder(a,s.gethead());
	for(int i = 0; i <= n - 1; i++){
		std::cout<<a[i]<<' ';//输出排列好的a数组 
	}
	return 0;
}
