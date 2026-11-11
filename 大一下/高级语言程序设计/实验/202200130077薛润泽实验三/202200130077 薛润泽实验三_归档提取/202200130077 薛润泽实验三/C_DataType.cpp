#include<iostream>
using namespace std;
class DataType {
public:
	DataType(char);
	DataType(int);
	DataType(float);
	void print();
private:
	int sym = 0;
	char a;
	int b;
	float c;
};
DataType::DataType(char m) {
	sym = 1;
	a = m;
}
DataType::DataType(int m) {
	sym = 2;
	b = m;
}
DataType::DataType(float m) {
	sym = 3;
	c = m;
}
void DataType::print() {
	if (sym == 1)cout <<"char:" << a << endl;
	else if (sym == 2)cout <<"int:"<<b << endl;
	else if (sym == 3) cout <<"float:"<< c << endl;
}

int main() {
	DataType aa('e'), bb(16), cc(2.56F);
	aa.print();
	bb.print();
	cc.print();
	return 0;
}