#include<iostream>

using namespace std;

class Point {
public:
	Point(int _x, int _y) :_x(_x), _y(_y) {};
	void show();
	Point operator++(int);
	Point operator--(int);
	Point operator++();
	Point operator--();
private:
	int _x;
	int _y;
};
void Point::show() {
	cout << "_x:" << _x << "  _y:" << _y << endl;
}

Point Point::operator++(int) {
	Point old = *this;
	this->_x++;
	this->_y++;
	return old;
}
Point Point::operator--(int) {
	Point old = *this;
	this->_x--;
	this->_y--;
	return old;
}
Point Point::operator++() {
	this->_x++; this->_y++;
	return *this;
}
Point Point::operator--() {
	this->_x--; this->_x--;
	return *this;
}
int main() {
	Point p1(1, 3);
	cout << "p1=  ";
	p1.show();
	p1++;
	cout << "p1++:";
	p1.show();
	p1--;
	cout << "p1--:";
	p1.show();
	++p1;
	cout << "++p1:";
	p1.show();
	--p1;
	cout << "--p1:";
	p1.show();
	return 0;
}