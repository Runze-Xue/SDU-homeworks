#include<iostream>
using namespace std;
int maxl(int x, int y) {
	if (x > y) return x;
	else return y;
}
int max1(int x, int y, int z) {
	int m1 = x > y ? x : y;
	return m1 > z ? m1: z;
}
double maxl(double x, double y) {
	return x > y ? x : y;
}
double  max1(double x, double y, double z) {
	double m1 = x > y ? x : y;
	return m1 > z ? m1 : z;
}
int main() {
	int num1 = 3, num2 = 4, num3 = 5;
	double dnum1 = 8.934, dnum2 = 6.8343, dnum3 = 123.3435;
	cout << maxl(num1,num2) << endl;
	cout << max1(num1, num2, num3) << endl;
	cout << maxl(dnum1, dnum2) << endl;
	cout << max1(dnum1, dnum2, dnum3)<<endl;
	return 0;
}