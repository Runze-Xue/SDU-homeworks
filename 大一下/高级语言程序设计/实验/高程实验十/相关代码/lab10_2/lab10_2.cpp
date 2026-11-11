#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main() {
	vector<int> A(8);
	cout << "给vector数组赋值 6, 7, 8, 9, 0, 1, 2, 3" << endl;
	for (int i = 0; i < 8; i++) {
		A[i] = (i + 6) % 10;
	}
	cout << "查找9，并返回下标：" << endl;
	cout<<find(A.begin(), A.end(), 9)-A.begin();
	cout << endl;
	cout << "数组升序排序,并输出" << endl;
	sort(A.begin(), A.end());
	for (int i = 0; i < 8; i++) {
		cout << A[i] << "  ";
	}
	cout << endl;
	cout << "数组降序排序，并输出" << endl;
	sort(A.begin(), A.end(), greater<int>());
	for (int i = 0; i < 8; i++) {
			cout << A[i] << "  ";
		}
	return 0;
}