#include<iostream>
using namespace std;
int main() {
	int a = 0;
	cin >> a;
	while (a != 1) {
		if (a % 2 != 0) {
			cout << a << " * 3 + 1 =" ;
			a = a * 3 + 1;
			cout << a << endl;
		}
		else {
			cout << a << " / 2 = "  ;
			a = a / 2;
			cout << a << endl;
		}
	}
	cout << "End"<<endl;
	return 0;
}
