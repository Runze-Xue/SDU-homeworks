#include<iostream>
using namespace std;
int  rabit(int n) {
	if (n == 0) return 1;
	if (n == 1) return 1;
	return rabit(n - 1) + rabit(n - 2);
}
int main() {
	
	int n = 0;
	cin >> n;
	cout << rabit(n);
	return 0;
}