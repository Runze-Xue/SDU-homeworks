#include<iostream>

struct Node{
	Node(int v){
		leftptr = nullptr;
		rightptr = nullptr;
		value = v;
		LeftSize = 0;
	}
	int value;
	int LeftSize = 0;
	Node* leftptr = nullptr;
	Node* rightptr = nullptr;
};
class  SearchTree{
public:
	SearchTree(){
		root = nullptr;
	}
	int push(Node*& cnt, int v,int ans = 0 ){//cnt是一个引用 
		if(cnt == nullptr){cnt = new Node(v); return ans;}//如果cnt == nullptr，则进行插入 
		else if(cnt->value == v) return -1;//原来树中已经存在v值 
		else if(cnt->value < v) push(cnt->rightptr, v ,ans ^ cnt->value);
		else {//插入到左子树，RankNode++ 
		int finial = push(cnt->leftptr, v, ans ^ cnt->value);
		if(finial != -1) cnt->LeftSize++;//如果插入成功，更新LeftSize值 
		return finial;}
	}
	void Push(int v){
		int ans = push(root, v);
		if(ans == -1)std::cout<<"0"<<'\n';//ans = -1 ，树中存在元素，输出0 
		else std::cout<<ans<<'\n';
		return;
	}
	int search(Node*&cnt, int v, int ans = 0){
		if(cnt == nullptr) return 0;//没找到，返回0  
		else if(cnt->value == v) return ans ^ v;//找到了，返回异或值 
		else if(cnt->value < v) return search(cnt->rightptr, v, ans ^ cnt->value);
		else return search(cnt->leftptr, v, ans ^ cnt->value);
	}
	void Search(int v){
		int ans = search(root, v);
		std::cout<<ans<<'\n';
	}
	Node*& findMin(Node*& cnt){//开始时cnt是待删除节点的右子树根节点, 保证初始不为空 
		if(cnt ->leftptr == nullptr) return cnt;
		cnt->LeftSize--;
		findMin(cnt->leftptr);
	}
	int deleteElement(Node*&cnt, int v, int ans = 0){
		if(cnt == nullptr)return -1;
		else if(cnt->value < v) deleteElement(cnt->rightptr, v, ans ^ cnt->value);
		else if(cnt->value > v) {
			int finial = deleteElement(cnt->leftptr, v, ans ^ cnt->value);
			if(finial != -1) cnt->LeftSize--;//如果成功删除某点，则LeftSize-- 
			return finial;
		}
		else if(cnt->value == v){//找到了要删除点 
			ans = ans ^ cnt->value;
			if(cnt->leftptr == nullptr && cnt->rightptr == nullptr){//删除点为叶子节点 
				delete cnt;
				cnt = nullptr;
			}
			else if(cnt->leftptr != nullptr && cnt->rightptr == nullptr){//有一个孩子 
				Node* temp = cnt;
				cnt = cnt->leftptr;
				delete temp;
			}
			else if(cnt->leftptr == nullptr && cnt->rightptr != nullptr){
				Node* temp = cnt;
				cnt = cnt->rightptr;
				delete temp;
			}			
			else if(cnt->leftptr != nullptr && cnt->rightptr != nullptr){//有两个孩子 
				Node*& Mintemp1 = findMin(cnt->rightptr);//Mintemp是最小节点的指针引用
				Node* Mintemp = Mintemp1;
				Mintemp1 = Mintemp1->rightptr;//将原来父节点的右指针指向被替换点的右结点 
				Node* temp = cnt;//记录cnt指针，用于释放原cnt
				Mintemp->leftptr = cnt->leftptr;
				Mintemp->rightptr = cnt->rightptr;
				Mintemp->LeftSize = cnt->LeftSize; 
				cnt = Mintemp;
				delete temp;
			}
			return ans;
		}
	}
	void DeleteElement(int v){
		int ans = deleteElement(root, v);
		if(ans == -1) std::cout<<"0"<<'\n';
		else std::cout<<ans<<'\n';
	}
	int RankSearch(Node*cnt, int rank, int ans = 0){
		if(cnt == nullptr) return 0;//没找到
		if(cnt->LeftSize + 1 > rank) RankSearch(cnt->leftptr, rank, ans^cnt->value);
		else if(cnt->LeftSize + 1 < rank) RankSearch(cnt->rightptr, rank - cnt->LeftSize - 1,ans^cnt->value);
		else return ans^ cnt->value;//找到 
	}
	void rankSearch(int rank){
		int temp = RankSearch(root, rank);
		std::cout<<temp<<'\n';
//		std::cout<<RankSearch(root, rank)->value<<'\n';
	}
	
	int RankDelete(Node*& cnt, int rank, int ans = 0){
		if(cnt == nullptr)return -1;//没找到,删除失败
		if(cnt->LeftSize + 1 > rank){//当前节点序号比要找的序号大 
			int tempans = RankDelete(cnt->leftptr, rank, ans^cnt->value);//向当前节点的左侧寻找 
			if(tempans != -1) cnt->LeftSize--;//如果删除成功，则更新LeftSize值 
			return tempans;
		}
		else if(cnt->LeftSize + 1 < rank) RankDelete(cnt->rightptr, rank - cnt->LeftSize - 1, ans^cnt->value);//当前节点比要找的序号小，向右寻找 
		else{//找到, 对deleteElement代码删除操作复用 
			ans = ans ^ cnt->value;
			if(cnt->leftptr == nullptr && cnt->rightptr == nullptr){//删除点为叶子节点 
				delete cnt;
				cnt = nullptr;
			}
			else if(cnt->leftptr != nullptr && cnt->rightptr == nullptr){//有一个孩子 
				Node* temp = cnt;
				cnt = cnt->leftptr;
				delete temp;
			}
			else if(cnt->leftptr == nullptr && cnt->rightptr != nullptr){
				Node* temp = cnt;
				cnt = cnt->rightptr;
				delete temp;
			}			
			else if(cnt->leftptr != nullptr && cnt->rightptr != nullptr){//有两个孩子 
				Node*& Mintemp1 = findMin(cnt->rightptr);//Mintemp是最小节点的指针引用
				Node* Mintemp = Mintemp1;
				Mintemp1 = Mintemp1->rightptr;//引用置为空，原来父节点的leftptr 或者rightptr则为空 
				Node* temp = cnt;
				Mintemp->leftptr = cnt->leftptr;
				Mintemp->rightptr = cnt->rightptr;
				Mintemp->LeftSize = cnt->LeftSize; 
				cnt = Mintemp;
				delete temp;
			}
			return ans;	
		} 
	}
	void rankDelete(int rank){
		int ans = RankDelete(root, rank);
		if(ans == -1)std::cout<<"0"<<'\n';
		else std::cout<<ans<<'\n';
	}
	void preorder(Node* cnt){//测试用输出 
		if(cnt == nullptr) return ;
		std::cout<<cnt->value<<' ';
		preorder(cnt->leftptr);
		preorder(cnt->rightptr);
	}
	void preLeftSize(Node* cnt){
		if(cnt == nullptr)return;
		std::cout<<cnt->LeftSize<<' ';
		preLeftSize(cnt->leftptr);
		preLeftSize(cnt->rightptr);
	}
	void Preorder(){
		preorder(root);
		std::cout<<'\n';
		preLeftSize(root);
	}
private:
	Node *root;
};
int main(){
	std::ios::sync_with_stdio(false);//解除std::cin的缓冲区绑定，加快std::cin的速度 
	std::cin.tie(nullptr);
	SearchTree tree;
	int m;
	std::cin>>m;
	int a, b;
	for(int i = 1; i<= m; i++){
		std::cin>>a>>b;
		if(a == 0){
			tree.Push(b);
		}
		else if(a == 1){
			tree.Search(b);
		}
		else if(a == 2){
			tree.DeleteElement(b);
		}
		else if(a == 3){
			tree.rankSearch(b);
		}
		else if(a == 4){
			tree.rankDelete(b);
		} 
	} 
	return 0;
}
