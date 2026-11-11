#include<iostream>

class Adjacency_matrix{//邻接矩阵类 
public:
	Adjacency_matrix(int n){
		Matrix_Size = n;
		element = new bool*[n - 1];
		//去除对角线，有n-1行，n列 
		for(int i = 0; i < n-1; i++){
			element[i] = new bool[n] ;
			for(int j = 0; j< n ; j++){
				element[i][j] = 0;
			}
		}
	}
	~Adjacency_matrix(){
		delete []element;
	}
	void set(int i, int j){//i j 从1 开始， 但是数组从 0 开始 
		if(i == j) return;//不形成自环
		else if(i > j) element[i - 1][j - 1] = 1; 
		else if(i < j) element[i - 1][j - 2] = 1;//去除对角线后，对角线下方元素向上平移1 
	}
	bool get(int i, int j){
		if(i == j) return 0;
		else if(i > j) return element[i - 1][j - 1]; 
		else if(i < j) return element[i - 1][j - 2];
	}
private:
	bool** element;//数组中不记对角线 
	int Matrix_Size;
};
int main(){
	Adjacency_matrix m(10);//以一个10*10的邻接矩阵为例 
	std::cout<<m.get(1,2);//查找点1， 2单向路径，无单向路径，输出0 
	m.set(1,2);//更新点1,2之间有单向路径 
	std::cout<<m.get(1,2);//输出1 
	return 0;
}
