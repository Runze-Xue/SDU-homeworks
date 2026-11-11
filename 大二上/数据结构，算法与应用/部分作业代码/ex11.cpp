#include<iostream>

int Read(){
    int v = 0;
    char c = getchar();
    while(c > '9' || c< '0'){
        c = getchar();
    }
    while(c >='0'&& c <= '9'){
        v = 10*v + c-'0';
        c = getchar();
    }
    return v;
}
template<typename T>
struct Node{
	T value;
	int SumNodenum = 0;
	int LeftNodenum = 0;
	Node* leftptr = nullptr;
	Node* rightptr = nullptr;
};
template<typename T>
class SearchTree{
public:
	SearchTree(){
		root = nullptr;
	}
	void deletetree(Node<T>* cnt){
		if(cnt == nullptr){
			return;
		}
		if(cnt->leftptr != nullptr) deletetree(cnt->leftptr);
		if(cnt->rightptr != nullptr) deletetree(cnt->rightptr);
		delete cnt;
	}
	~SearchTree(){
		deletetree(root);
	}
	void push(T v){//函数插入操作 
		Node<T>* temp = new Node<T>;
		temp->leftptr = nullptr;
		temp->rightptr = nullptr;
		temp->value = v;
		if(root == nullptr){
			root = temp;
            std::cout<<"0"<<'\n';
			return;
		}
		Node<T>* cnt = root;
		Node<T>* last = nullptr;
		int ans = 0;
		while(cnt != nullptr){
			if(cnt->value == v){
				std::cout<<"0"<<'\n';//已找到，输出0 
				delete temp;
				return;
			}		
			else if(cnt->value < v){
				ans ^= cnt->value;
				last = cnt;
				cnt = cnt->rightptr;
			}
			else if(cnt->value > v){
				ans ^= cnt->value;
				last = cnt;
				cnt = cnt->leftptr;
			}	
		}//当cnt == nullptr时退出循环，说明已经遍历到空节点，last为根节点 
		if(last->value < v){
			last->rightptr = temp;
		}
		else if(last->value > v){
			last->leftptr = temp;
		}
		std::cout<<ans<<'\n';
	}
	void search(int v){
		int ans = 0;
		Node<T>* cnt = root;
		while(cnt != nullptr){
			ans ^= cnt->value;
			if(cnt->value == v){
				std::cout<<ans<<'\n';
				return; 
			}
			else if(cnt->value < v){
				cnt = cnt->rightptr;
			}
			else if(cnt->value > v){
				cnt = cnt->leftptr;
			}
		}
		std::cout<<"0"<<'\n';
	}
	void deleteElement(int v){
		int ans = 0;
		Node<T>* cnt = root;
		Node<T>* last = nullptr;
		while(cnt != nullptr){
			if(cnt->value == v){
				ans ^= v;//待查询的元素异或入ans中 
				std::cout<<ans<<'\n';
				if(cnt->leftptr == nullptr &&cnt->rightptr == nullptr){//待删除节点是叶子节点 
					if(cnt == root){//删除节点为根节点 
						delete cnt; root = nullptr;cnt = nullptr;
						return;
					}
					if(last->value < v){//cnt->value = v， cnt为last的右孩子 
						last->rightptr = nullptr;
						delete cnt; cnt = nullptr;
					}
					else if(last->value > v){//cnt为last的左孩子 
						last->leftptr = nullptr;
						delete cnt;cnt = nullptr;
					}
                }
				else if(cnt != nullptr && cnt->leftptr==nullptr && cnt->rightptr != nullptr){//待删除节点无左孩子，有右孩子 
					if(cnt == root){//删除节点为根节点 
						root = cnt->leftptr;
						delete cnt;cnt = nullptr;
						return;
					}
					if(cnt!=nullptr && last->value < v) last->rightptr = cnt->rightptr;//cnt节点是last的右节点的情况 
					else if(cnt!=nullptr && last->value > v)last->leftptr = cnt->rightptr;//cnt节点是last的左节点的情况 
					delete cnt; cnt = nullptr;//删除cnt节点 
				}
				else if(cnt!=nullptr && cnt->leftptr!=nullptr && cnt->rightptr == nullptr){//待删除节点有左孩子，无右孩子 
					if(cnt == root){//删除节点为根节点
						root = cnt->leftptr;
						delete cnt; cnt = nullptr;
						return;
					}
					if(cnt!=nullptr && last->value < v) last->rightptr = cnt->leftptr;
					else if(cnt != nullptr && last->value > v)last->leftptr = cnt->leftptr;
				}
				else{//待删除节点有两个孩子 
					Node<T> * temp = cnt->rightptr;//找右节点的最小值
					Node<T> * templast = cnt;
					while(temp->leftptr != nullptr){
					   	templast = temp;
					    temp = temp->leftptr;
					}
						if(cnt == root){//删除节点为根节点
							root = temp; 
						}
						else {//删除节点不是根节点
							if(last->value < temp->value) last->rightptr = temp;
							else last->leftptr = temp;
						}
						if(temp->rightptr == nullptr){//如果替换点是叶子节点 
							temp->leftptr = cnt->leftptr;
							if(cnt->rightptr != temp) temp->rightptr = cnt->rightptr;//防止形成自连 
						}
						else{//替换点不是叶子节点，而是有右孩子的节点
							if(templast == cnt){//如果替换点是右子树根节点 
								temp->leftptr = cnt->leftptr;
							} 
							else{//替换点不是右子树根节点 
								templast->leftptr = temp->rightptr;
								temp->leftptr = cnt->leftptr;
								temp->rightptr = cnt->rightptr;
							}
						}
						delete cnt; cnt = nullptr;//对以上三种情况删除cnt 
				}
				return;
			}
			else if(cnt != nullptr && cnt->value < v){
				ans ^= cnt->value;
				last = cnt;
				cnt = cnt->rightptr;
			}
			else if(cnt!=nullptr && cnt->value > v){
				ans ^= cnt->value;
				last = cnt;
				cnt = cnt->leftptr; 
			}
		}
		std::cout<<"0"<<'\n';
		return;
	}
	void Update(){
		updateSumNode(root);
		updateLeftNode(root);
	}
	void updateSumNode(Node<T>* cnt){
		if(cnt->leftptr == nullptr && cnt->rightptr == nullptr){
			cnt->SumNodenum = 1;
			return;
		}
		if(cnt->leftptr != nullptr) updateSumNode(cnt->leftptr);//更新左子树
		if(cnt->rightptr != nullptr) updateSumNode(cnt->rightptr);//更新右子树
		int templeft , tempright;//更新cnt节点 
		if(cnt->leftptr == nullptr) templeft = 0;
		else if(cnt->leftptr != nullptr) templeft = (cnt->leftptr)->SumNodenum;
		if(cnt->rightptr == nullptr) tempright = 0;
		else if(cnt->rightptr != nullptr) tempright = (cnt->rightptr)->SumNodenum;
		cnt->SumNodenum = templeft + tempright + 1;  	
	}
	void updateLeftNode(Node<T>*cnt){
		if(cnt == nullptr) return;
		updateLeftNode(cnt->leftptr);
		updateLeftNode(cnt->rightptr);
		if(cnt->leftptr != nullptr)cnt->LeftNodenum = (cnt->leftptr)->SumNodenum;
		else cnt->LeftNodenum = 0;
	}
	//输出检验 
	void preOrder(){//前序遍历 
		PreOrder(root);std::cout<<'\n';
		PreOrderSumNode(root);std::cout<<'\n';
		PreOrderLeftNode(root);std::cout<<'\n';
	}
	void PreOrder(Node<T>* cnt){
		if(cnt == nullptr) return;
		std::cout<<cnt->value<<' ';
		PreOrder(cnt -> leftptr);
		PreOrder(cnt -> rightptr);
	}
	void PreOrderSumNode(Node<T>* cnt){
		if(cnt == nullptr)return;
		std::cout<<cnt->SumNodenum<<' ';
		PreOrderSumNode(cnt->leftptr);
		PreOrderSumNode(cnt->rightptr);
		return;
	}
	void PreOrderLeftNode(Node<T>* cnt){
		if(cnt == nullptr)return;
		std::cout<<cnt->LeftNodenum<<' ';
		PreOrderLeftNode(cnt->leftptr);
		PreOrderLeftNode(cnt->rightptr);
		return;
	}

void searchRank(int r, int sym = 0){//sym = 0 是查询操作, sym = 1 是删除操作 
		if(root == nullptr){//树中无节点，直接返回 并输出 0  
			std::cout<<"0"<<'\n';
			return;
		}
		Node<T>* last = nullptr;
		Node<T>* cnt = root;
		int rankcnt =  root->LeftNodenum + 1;//rankcnt来记录当前节点的次序 
		int acs = 0;//记录一定大于的序号，在进入右子树时更新
		int ans = 0;//记录异或值  
		//在每次遍历子树前进行异或，所以等到查询到相应元素时，也要进行一次异或 
		while(cnt != nullptr){
			ans = ans ^ cnt->value;
			if(rankcnt > r){//要转到左子树寻找
				last = cnt;
				cnt = cnt->leftptr;
				if(cnt == nullptr){
					std::cout<<"0"<<'\n';
					return;
				}
				rankcnt = cnt->LeftNodenum + 1 + acs;
			}
			else if(rankcnt < r){//转到右子树寻找
				last = cnt;
				acs = acs + cnt->LeftNodenum + 1;
				cnt = cnt->rightptr;
				if(cnt == nullptr){
					std::cout<<"0"<<'\n';
					return;
				}
				rankcnt = acs + 1 + cnt->LeftNodenum;
			}
			else {//找到该节点 
				std::cout<<ans<<'\n';
				if(sym == 1){//如果是删除操作 
					//删除开始
				if(cnt->leftptr == nullptr &&cnt->rightptr == nullptr){//待删除节点是叶子节点 
					if(cnt == root){//删除节点为根节点 
						delete cnt; root = nullptr;
						return;
					}
					if(last->value < cnt->value){//cnt->value = v， cnt为last的右孩子 
						last->rightptr = nullptr;
						delete cnt; 
					}
					else if(last->value > cnt->value){//cnt为last的左孩子 
						last->leftptr = nullptr;
						delete cnt;
					}
                }
				else if(cnt->leftptr==nullptr && cnt->rightptr != nullptr){//待删除节点无左孩子，有右孩子 
					if(cnt == root){//删除节点为根节点 
						root = cnt->leftptr;
						delete cnt;
						return;
					}
					if(last->value < cnt->value) last->rightptr = cnt->rightptr;//cnt节点是last的右节点的情况 
					else if(last->value > cnt->value)last->leftptr = cnt->rightptr;//cnt节点是last的左节点的情况 
					delete cnt;//删除cnt节点 
				}
				else if(cnt->leftptr!=nullptr && cnt->rightptr == nullptr){//待删除节点有左孩子，无右孩子 
					if(cnt == root){//删除节点为根节点 
						root = cnt->leftptr;
						delete cnt;
						return;
					}
					if(last->value < cnt->value) last->rightptr = cnt->leftptr;
					else if(last->value > cnt->value)last->leftptr = cnt->leftptr;
				}
				else{//待删除节点有两个孩子 
					Node<T> * temp = cnt->rightptr;//找右节点的最小值
					Node<T> * templast = cnt;
					while(temp->leftptr != nullptr){
					   	templast = temp;
					    temp = temp->leftptr;
					}
						if(cnt == root){//删除节点为根节点
							root = temp; 
						}
						else {//删除节点不是根节点
							if(last->value < temp->value) last->rightptr = temp;
							else last->leftptr = temp;
						}
						if(temp->rightptr == nullptr){//如果替换点是叶子节点 
							temp->leftptr = cnt->leftptr;
							if(cnt->rightptr != temp) temp->rightptr = cnt->rightptr;//防止形成自连 
						}
						else{//替换点不是叶子节点，而是有右孩子的节点
							if(templast == cnt){//如果替换点是右子树根节点 
								temp->leftptr = cnt->leftptr;
							} 
							else{//替换点不是右子树根节点 
								templast->leftptr = temp->rightptr;
								temp->leftptr = cnt->leftptr;
								temp->rightptr = cnt->rightptr;
							}
						}
						delete cnt;//对以上三种情况删除cnt 
				}
				//删除结束
				}
				return ;
			} 
		}
		std::cout<<"0"<<'\n';
		return;
	}
private:
	Node<T>* root = nullptr;	
};
int main(){
	SearchTree<int> tree;
	int m = Read();
	int a, b;
	for(int i = 1; i<=m; i++){
		a = Read(); b = Read();
		if(a == 0){
			tree.push(b);
		}
		else if(a == 1){
	//		tree.search(b);
		}
		else if(a == 2){
			tree.deleteElement(b);
		}
		else if(a == 3){
			tree.Update();
	//		tree.searchRank(b);
		}
		else if(a == 4){
			tree.Update();
	//		tree.searchRank(b,1);
		}
	}
	return 0; 
}
