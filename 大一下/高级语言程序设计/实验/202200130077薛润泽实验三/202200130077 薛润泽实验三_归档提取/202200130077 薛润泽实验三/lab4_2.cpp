#include<iostream>
using namespace std;
enum CPU_Rank { P1 = 1, P2, P3, P4, P5, P6, P7 };
class CPU {
public:
	CPU(CPU_Rank r, int fr, double vol);
	CPU() {}
	void run();
	void stop();
	~CPU() {
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
	cout << "调用了构造函数" << " rank初始状态为 " << rank << " frequency初始状态为 " << frequency << " voltage初始状态为 " << voltage << endl;
}
void CPU::run() {
	cout << "CPU类run了" << endl;
}
void CPU::stop() {
	cout << "CPU类stop了" << endl;
}
enum TYPE{DDR4=1,DDR3,DDR2,DDR1};
class RAM {
public:
	void run();
	void stop();
private:
	double capacity;
	TYPE type;
	int frequency;
};
void RAM::run() {
	cout << "RAM类run了" << endl;
}

void RAM::stop() {
	cout << "RAM类stop了" << endl;
}
enum PLUGETYPE{SATA=1,USB,USB2,USB3};
enum INSTALLWAYS{external=1,build_in};
class CDROM {
public:
	void run();
	void stop();
private:
	PLUGETYPE pluge;
	double capacity;
	INSTALLWAYS installways;
	};
void CDROM::run() {
	cout << "CDROM类run了" << endl;
}
void CDROM::stop() {
	cout << "CDROM类stop了" << endl;
	}
class Computer {
public:
	void run();
	void stop();
private:
	CPU cpu;
	RAM ram;
	CDROM cdrom;
};
void Computer::run() {
	cout << "Computer类run了" << endl;
	cpu.run();
	ram.run();
	cdrom.run();
}
void Computer::stop() {
	cout << "Computer类stop了" << endl;
	cpu.stop();
	ram.stop();
	cdrom.stop();
	}


int main() {
	Computer computer;
	computer.run();
	computer.stop();
	return 0;
}