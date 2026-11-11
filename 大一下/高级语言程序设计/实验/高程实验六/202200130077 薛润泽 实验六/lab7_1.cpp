#include<iostream>

using namespace std;

class Animal{
public:
	int age;
};
class dog:public Animal {
public:
	void SetAge(int n) {
		age = n;
		cout << age << endl;
	}
};
int main() {
	dog dg;
	dg.SetAge(2);
	return 0;
}