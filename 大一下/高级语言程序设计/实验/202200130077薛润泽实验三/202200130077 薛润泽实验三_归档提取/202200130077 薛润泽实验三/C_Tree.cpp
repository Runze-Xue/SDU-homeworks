#include<iostream>

using namespace std;

class Tree {
public:
	Tree(int a);
	void grow(int years);
	void age();
private:
	int ages;
};
Tree::Tree(int a) {
	ages = a;
}
void Tree::grow(int years) {
	ages += years;
	cout << "The age of this tree is " << ages << endl;
}
void Tree::age() {
	cout << "The age of this tree is " << ages << endl;
}
int main() {
	Tree t(12);
	t.age();
	t.grow(4);
	return 0;
}