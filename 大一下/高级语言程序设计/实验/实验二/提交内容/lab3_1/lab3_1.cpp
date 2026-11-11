#include<iostream>
using namespace std;
void tmpturn(double F) {
	double C;
	C = (F - 32) * 5 / 9;
	cout << C << endl;
}
int main() {
	double F;
	cin >> F;
	tmpturn(F);
	return 0;
}