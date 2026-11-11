#include<iostream>
#include<cstdlib>
using namespace std;
enum GameStatus{WIN,LOSE,PLAYING};
class C_DICE {
public:
	C_DICE() {};
	void start(int s);
	void  deal();
	void judge();
	void show();
private:
	int sym = 1;
	unsigned seed;//开始随机种子
	int aimstatus=0;//
	int sum=0;//胜利判断
	GameStatus gamestatus=PLAYING;
};
void C_DICE::start(int s) {
	 srand(s);
}
void C_DICE::judge() {
	if (gamestatus != PLAYING) return;
	if (sym == 1) {
		if (sum == 7 || sum == 11) gamestatus=WIN;
		else if (sum == 2 || sum == 3 || sum == 12) gamestatus=LOSE;
		else {
			sym++;
			aimstatus = sum;
			deal();
		}
	}
	else {
		if (sum == aimstatus) gamestatus = WIN;
		else	if (sum != aimstatus && sum == 7) gamestatus = LOSE;
		else {
			aimstatus = sum;
			deal();
		}
	}
}
void C_DICE::deal() {
	if (gamestatus != PLAYING) return;
	int rand1 = 1 + rand() % 6;
	int rand2 = 1 + rand() % 6;
	sum = rand1 + rand2;
	cout << "PLAYER ROLLED " << rand1 << "+" << rand2 << "=" << sum << endl;
	judge();
	return;
}
void C_DICE::show(){
	if (gamestatus == WIN) {
		cout << "PLAYER WINS";
	}
	else if (gamestatus == LOSE) {
		cout << "PLAYER LOSES";
	}
	else if (gamestatus == PLAYING) {
		cout << "PLAYING PLAYS";
	}
}
int main() {
	int seed;
	cin >> seed;
	C_DICE cdice;
	cdice.start(seed);
	cdice.deal();
	cdice.show();
	return 0;
}