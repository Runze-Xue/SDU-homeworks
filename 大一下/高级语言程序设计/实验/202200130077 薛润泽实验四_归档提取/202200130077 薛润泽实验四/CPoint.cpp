#include<iostream>
#include<math.h>

using namespace std;

class CPoint {
private:
	double x;//横坐标
	double y;//纵坐标
public:
	CPoint(double xx = 0, double yy = 0) :x(xx), y(yy) {};
	double getx();
	double gety();
	void distance(); //成员函数
	friend void distance2(CPoint& p1);//友元函数

};
double CPoint::getx() {
	return x;
}
double CPoint::gety() {
	return y;
}
//成员函数计算距离
void CPoint :: distance() {

	cout << "成员函数计算距离：" << sqrt(x * x + y * y) << endl;
}
//友元函数计算距离
void distance2(CPoint& p1) {
	cout <<"友元函数计算距离："<< sqrt(p1.x * p1.x + p1.y * p1.y)<<endl;
}

//一般函数计算距离
void distance3(CPoint& p) {
	int p1 = p.getx();
	int p2 = p.gety();
	cout << "一般函数计算距离：" << sqrt(p1 * p1 + p2 * p2)<<endl;
}

int main() {
	CPoint cpoint(6, 9);
	cpoint.distance();//调用成员函数
	distance2(cpoint);//调用友元函数
	distance3(cpoint);//调用一般函数
	return 0;
}