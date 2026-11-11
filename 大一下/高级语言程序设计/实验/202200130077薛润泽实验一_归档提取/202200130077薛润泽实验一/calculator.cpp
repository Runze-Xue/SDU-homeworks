#include<iostream>
using namespace std;
int main() {
	char sym;
	double left, right;
	cin >> left >>sym>> right ;
	if (sym == '*') {
		cout << left << " * " << right << " = " << left * right;

	}
	else if (sym == '+') {
		cout << left << " + " << right << " = " << left + right;
	}
	else if (sym == '-') {
		cout << left << " - " << right << " = " << left - right;
	}
	else if (sym == '/') {
		if (right == 0) {
			cout << "除数不能为0";
			return 0;
		}
		else 	cout << left << " / " << right << " = " << left / right;
	}
	return 0;
}