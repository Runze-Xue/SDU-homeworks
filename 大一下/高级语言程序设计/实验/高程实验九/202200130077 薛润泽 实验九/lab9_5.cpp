#include"Array.h"
#include<iostream>

using namespace std;

int main() {
	Array<int> ar(8);
	cout << "第一次数组初始状态：";
	for (int i = 0; i < 8; i++) {
		ar[i] = 10 - i;	
		cout << ar[i] << " ";
	}
	cout << endl<<"插入排序后状态：";
	ar.insertsort();
	for (int i = 0; i < 8; i++) {
		cout << ar[i] << " ";
	}
	cout <<endl <<"第二次数组初始状态：";
	for (int i = 0; i < 8; i++) {
		ar[i] = 19 - i;
		cout << ar[i] << " ";
	}
	cout << endl<<"选择排序后状态：";
	ar.selectsort();
	for (int i = 0; i < 8; i++) {
		cout << ar[i] << " ";
	}
	cout << endl<<"第三次数组初始状态：";
	for (int i = 0; i < 8; i++) {
		ar[i] = 90 - i;
		cout << ar[i] << " ";
	}
	ar.bobblesort();
	cout <<endl<< "冒泡排序后状态：";
	for (int i = 0; i < 8; i++) {
		cout << ar[i] << " ";
	}
	cout <<endl <<"在冒泡排序后数组中查找89，并返回下标:" << endl;
	cout << ar.linearsearch(89)<<endl;
	return 0;
}
