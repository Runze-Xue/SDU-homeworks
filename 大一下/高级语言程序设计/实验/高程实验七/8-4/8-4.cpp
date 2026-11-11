#include<iostream>

using namespace std;

class Counter {
public:
	Counter(double data = 0.0,double ca=0.0) :data(data),ca(ca) {};
	void show() {
		cout << data << "," << ca << endl;
	}
	Counter operator+(const Counter&A)const;
private:
	double data;
	double ca;
};
Counter Counter::operator+(const Counter &A)const {
	return Counter(this->data + A.data, this->ca + A.ca);
}

int main() {
	Counter c1(1, 2);
	Counter c2(4, 5);
	c1 = c1 + c2;
	c1.show();
	return 0;
}
