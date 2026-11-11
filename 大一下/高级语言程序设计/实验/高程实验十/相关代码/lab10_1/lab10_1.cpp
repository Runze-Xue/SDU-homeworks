#include<deque>
#include<iostream>

using namespace std;

int main() {
	deque<int> A;
	deque<int> B;
	for (int i = 0; i < 5; i++) {
		int insert1 = 0;
		cin >> insert1;
		A.push_back(insert1);
	}
	for (int i = 0; i < 5; i++) {
		int insert1 = 0;
		cin >> insert1;
		B.push_back(insert1);
	}
	for (int i = 0; i < 5; i++) {
		A.push_back(B.front());
		B.pop_front();
	}
	while (!A.empty()) {
		cout << A.front()<<"  ";
		A.pop_front();
	}
	return  0;
}