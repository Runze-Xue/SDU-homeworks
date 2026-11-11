#include<iostream>

using namespace std;

class vehicle {
public:
	void Run();
	void Stop();
protected:
	int MaxSpeed;
	int Weight;
};
void vehicle::Run() {
	cout << "该交通工具run了" << endl;
}
void vehicle::Stop() {
	cout << "该交通工具stop了" << endl;
}
class bicycle :virtual public vehicle {
protected:
	int Height;
};
class motorcar :virtual public vehicle {
protected:
	int SeatNum;
};
class motorcycle : public bicycle, public motorcar {

};

int main() {
	motorcycle mt;
	mt.Run();
	mt.Stop();
	return 0;
}
