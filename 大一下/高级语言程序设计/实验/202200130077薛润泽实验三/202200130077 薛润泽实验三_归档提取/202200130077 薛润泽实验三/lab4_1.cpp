#include<iostream>
using namespace std;
enum CPU_Rank{P1=1,P2,P3,P4,P5,P6,P7};
class CPU {
public:
	CPU(CPU_Rank r,int fr,double vol );
	void run();
	void stop();
	~CPU(){
		cout << "调用了析构函数" << endl;
	}
private:
	CPU_Rank rank;
	int frequency;
	double voltage;
};
CPU::CPU(CPU_Rank r, int fr, double vol) {
	rank = r;
	frequency = fr;
	voltage = vol;
	cout << "调用了构造函数" <<" rank初始状态为 " <<rank<<" frequency初始状态为 "<<frequency<<" voltage初始状态为 "<<voltage<< endl;
}
void CPU::run() {
	cout << "调用了run函数" << endl;
}
void CPU::stop() {
	cout << "调用了stop函数" << endl;
}
int main() {
	CPU cpu(P2,50,9.34);
	return 0;
}