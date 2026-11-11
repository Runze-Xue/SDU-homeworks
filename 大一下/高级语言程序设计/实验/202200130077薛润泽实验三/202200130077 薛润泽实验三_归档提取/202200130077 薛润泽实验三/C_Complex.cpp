#include<iostream>

using namespace std;

class Complex {
public:
	Complex(double a1, int b1);
	Complex(double a1);
	void add(Complex a1);
	void show();
private:
	double r1;
	int cp1;
};
Complex::Complex(double a1, int b1) {
	r1 = a1; cp1 = b1;
}
Complex::Complex(double a1) {
	r1 = a1;
}
void Complex::add(Complex a1) {
	r1 = r1 + a1.r1;
}
void Complex::show() {
	cout << r1 << "+" << cp1 << "i" << endl;
}
int main() {
	Complex c1(5, 8);
	Complex c2 = 6.7;
	c1.add(c2);
	c1.show();
	return 0;
}