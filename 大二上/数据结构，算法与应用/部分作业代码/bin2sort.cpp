#include<iostream>

using namespace std;

template<typename T>
class bin2sort{
public:
	bin2sort(int n):n(n){
		a = new T[n];
		for(int i = 0; i<n; i++){
			cin>>a[i];
		}
		for(int i = 0; i<=9; i++){
			bin[i] = new T[n];
		}	
	}
	~bin2sort(){
		delete []a;
		for(int i = 0; i<=9; i++){
			delete []bin[i];
		}
	}
	
private:
	T* a;//用来接收序列元素
	T* bin[10]; // 10箱子，用来排序 
	int n;//排序的元素个数 
}; 


int main(){
	bin2sort<int> s(5);
	return 0;
}
