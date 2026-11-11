#include<iostream>

using namespace std;

class vehicle {
public:
virtual	void Run();
virtual	void Stop();
};
void vehicle::Run() {
	cout << "vehicle类Run了" << endl;
}
void vehicle::Stop() {
	cout << "vehicle类Stop了" << endl;
}
class bicycle :virtual public vehicle {
public:
	void Run() {
		cout << "bicycle类Run了" << endl;
	}
	void Stop() {
		cout << "bicycle类Stop了" << endl;
	}
};
class motorcar :virtual public vehicle {
public:
	void Run() {
		cout << "motorcar类Run了" << endl;
	}
	void Stop() {
		cout << "motorcar类Stop了" << endl;
	}
};
class motorcycle :public bicycle, public motorcar {
public:
	void Run() {
		cout << "motorcycle类Run了" << endl;
	}
	void Stop() {
		cout << "motorcycle类Stop了" << endl;
	}
};
int main() {
	vehicle vh1;
	bicycle bc1;
	motorcar mt1;
	motorcycle my1;
	vehicle* vh2 = &vh1;
	vehicle* bc2 = &bc1;
	vehicle* mt2 = &mt1;
	vehicle* my2 = &my1;
	vh2->Run(); vh2->Stop();
	bc2->Run(); bc2->Stop();
	mt2->Run(); mt2->Stop();
	my2->Run(); my2->Stop();

}
