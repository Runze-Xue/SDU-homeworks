#pragma once
#ifndef _WARRIOR_
#define _WARRIOR_

#include<iostream>
#include<string.h>
#define sword  0
#define bomb  1
#define arrow  2

using namespace std;

void calchour(long long time) {
	long long hour = time / 60;
	int calcbit = 0;
	long long hour2 = hour;
	while (hour2 > 0) {
	 hour2=	hour2 / 10; calcbit++;//计算hour的位数有几位，
	}
	if (calcbit == 0)cout << "000";
	else if (calcbit == 1)cout << "00" << hour;
	else if (calcbit == 2)cout << "0" << hour;
	else cout << hour;
}
void calcminute(long long time) {
	long long minute = time % 60;
	int calcbit = 0;
	long long minute2 = minute;
	while (minute2 > 0) {
		minute2=minute2 / 10; calcbit++;//计算minute的位数有几位。
	}
	if (calcbit == 0) cout << "00";
	else if (calcbit == 1) cout << "0" << minute;
	else cout << minute;
}
//便捷化输出时间
void outtime(long long time) {
	calchour(time);
	cout << ":" ;
	calcminute(time);
	//输出形式 023:45 没有空格
}
void outcolor(long long colorr) {
	if (colorr == 0) cout << "red";
	else if (colorr == 1) cout << "blue";
	else cout << colorr<<"wrong color in outcolor";
}
void outprofession(int credit) {
	if (credit == 1)cout << "dragon";
	else if (credit == 2)cout << "ninja";
	else if (credit == 3)cout << "iceman";
	else if (credit == 4)cout << "lion";
	else if (credit == 5)cout << "wolf";
	else cout << "wrong credit";
}
class warrior {
public:
	int credit ;
	long long aim;// aim 用来检测是否到达对方司令部
	int color;//color = 0 是红武士 color = 1 是蓝武士
	int city;
	long long number; //warrior 的编号
	long long HP;//warrior 的生命值
	long long aggressivity=0;//warrior 的攻击力
	long long weapon[3][2];//warrior 的武器   注意sword 和 bomb只有[][1]被初始化使用， arrow可以使用次数为2的放在[][1]中，arrow可以使用次数为1的放在[][0]中
	long long keeparrow=0;//用来暂时保存耐久为2 使用后降为 1 的arrow
	int weaponline[10];//用来记录排序的武器
	int weaponsum=0;//用来记录总武器的件数，其中不包括keeparrow未进行更新的箭支，因为此时的所有keeparrow已经全部被更新
	//初始化weaponsum = 0 是因为keepA，keepB的初始值为0，所以一定能引起一次weaponupdate，则keepA，keepB再次变为0,weaponsum变为列表武器总和
	//注意weaponsum需要在武器排列时首先进行一次更新
	int symfight = 0;//用来检测是否参与了最近一次的战斗，如果没参与就是0，如果参与了就是1，用于欢呼后归零；主要用于武士dragon
	void gethurt(long long hurt) {
		HP = HP - hurt;
	}
	void cityplus() {
		city++;
	}
	void cityminor() {
		city--;
	}
	void setcity(int city1) {
		city = city1;
	}

	long long usesword() {
		return  aggressivity * 2 / 10;
	}
	//定义usebomb为虚函数，便于ninjia改写函数（ninjia不受到usebomb的伤害）
	virtual long long  usebomb() {
			long long hurt = aggressivity * 4 / 10;
			long long hurtself = hurt / 2;
			HP = HP - hurtself;
			weapon[bomb][1]--;
			return hurt;
	}
	virtual void cheer(long long time) {
		return ;
	}
	virtual void icemanwalk() {
		return;
	}
	virtual void changeloyal(int K) {
	//	cout << "base warrior 's changeloyal"<<endl;
		return;
	}
	virtual void wolfrobweapon(warrior* dr,long long recordtime) {
		return;
	}
	virtual void getweapon(warrior* dr) {
			dr->updatearrow();//将dr没有来得及更新的keeparrow进行更新
			long long countweapon = 0;
			countweapon = weapon[sword][1] + weapon[bomb][1] + weapon[arrow][0] + weapon[arrow][1];
			if (countweapon == 10)return;
			else if (countweapon > 10) {
				cout << "the warrior" <<credit <<number << "weapon over 10" << endl;
				return;
			}
			else if (countweapon < 10) {
				int leave = 10 - countweapon;//leave表示还能收集抢夺的道具数目
				while (leave > 0 && dr->weapon[sword][1] > 0) {
					weapon[sword][1]++;
					dr->weapon[sword][1]--;
					leave--;
				}
				while (leave > 0 && dr->weapon[bomb][1] > 0) {
					weapon[bomb][1]++;
					dr->weapon[bomb][1]--;
					leave--;
				}
				while (leave > 0 && dr->weapon[arrow][1] > 0) {
					weapon[arrow][1]++;
					dr->weapon[arrow][1]--;
					leave--;
				}
				while (leave > 0 && dr->weapon[arrow][0] > 0) {
					weapon[arrow][0]++;
					dr->weapon[arrow][0]--;
					leave--;
				}
			}
			return;
		
	}
	long long usearrow0() {
			weapon[arrow][0]--;
			return aggressivity * 3 / 10;
	}
	long long usearrow1() {
			weapon[arrow][1]--;
			keeparrow++;
			return aggressivity * 3 / 10;
	}
	void updatearrow() {
	weapon[arrow][0] += keeparrow;
	keeparrow = 0;
	}
	void createweapon(int n) {
		if (n == 0) weapon[sword][1]++;
		else if (n == 1) weapon[bomb][1]++;
		else if (n == 2) weapon[arrow][1]++;
	}
	long long checkweaponsum() {
		return (weapon[sword][1] + weapon[bomb][1] + weapon[arrow][0] + weapon[arrow][1] + keeparrow);
	}

	//用于给战斗时武器进行排序
	void countweapon() {
		updatearrow();//将未用的箭支更新,weaponsum用于记录排列表中所有武器数目，因为已经更新了箭支，所以不记录keeparrow(实际值为0)
		weaponsum = weapon[sword][1] + weapon[bomb][1] + weapon[arrow][0] + weapon[arrow][1];
		memset(weaponline, 0, sizeof(weaponline)); 
		weaponsum = weapon[sword][1] + weapon[bomb][1] + weapon[arrow][0] + weapon[arrow][1];
		int countAnum = 0; 
		for (int i = 0; i <weapon[sword][1]; i++) {
			weaponline[countAnum] = 1;
			countAnum++;
		}
		for (int i = 0; i < weapon[bomb][1]; i++) {
			weaponline[countAnum] = 2;
			countAnum++;
		}
		for (int i = 0; i < weapon[arrow][0]; i++) {
			weaponline[countAnum] = 3;
			countAnum++;
		}
		for (int i = 0; i < weapon[arrow][1]; i++) {
			weaponline[countAnum] = 4;
			countAnum++;
		}
	}
	virtual int getloyal() {
		cout << "Not inherited loyal" << endl;
		return 100;
	}
	virtual void reportsituation() {
		return;
	}
};

class dragon :public warrior {
public:
	friend class wolf;
	dragon(long long n,long long hp,long long colorr,long long time,long long agr)
	{
		aggressivity = agr;
		credit = 1;
		weapon[sword][1] = 0; weapon[bomb][1] = 0; weapon[arrow][0] = 0; weapon[arrow][1] = 0;//武器初始化
		color = colorr;//0是红，1是蓝
		HP = hp;
		number = n;
		createweapon(n % 3);
		outtime(time); cout << " ";
		outcolor(colorr); cout << " ";
		outprofession(credit); cout << " ";
		cout << number<<" born"<<endl;
	};
	void reportsituation() {
		if (color == 0)cout << " red dragon " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else if (color == 1)cout << " blue dragon " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else cout << "wrong color from dragon reportsituation" << endl;
		return;
	}
};

class ninjia :public warrior {
public:
	friend class wolf;
	ninjia(long long n,long long hp,long long colorr,long long time,long long agr) {
		aggressivity = agr;
		credit = 2;
		weapon[sword][1] = 0; weapon[bomb][1] = 0; weapon[arrow][0] = 0; weapon[arrow][1] = 0;//武器初始化
		color = colorr;
		HP = hp;
		number = n;
		createweapon(n % 3);
		createweapon((n + 1) % 3);
		outtime(time); cout << " ";
		outcolor(colorr); cout << " ";
		outprofession(credit); cout << " ";
		cout << number << " born"<<endl;
	}
	long long usebomb() {
		long long hurt = aggressivity * 4 / 10;
		weapon[bomb][1]--;
		return hurt;
	}
	void reportsituation() {
		if (color == 0)cout << " red ninja " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else if (color == 1)cout << " blue ninja " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else cout << "wrong color from ninja reportsituation" << endl;
		return;
	}
};

class iceman :public warrior {
public:
	friend class wolf;
	iceman(long long n,long long hp,long long colorr,long long time,long long agr) {
		aggressivity = agr;
		credit = 3;
		weapon[sword][1] = 0; weapon[bomb][1] = 0; weapon[arrow][0] = 0; weapon[arrow][1] = 0;//武器初始化
		color = colorr;
		HP = hp;
		number = n;
		createweapon(n % 3);
		outtime(time); cout << " ";
		outcolor(colorr); cout << " ";
		outprofession(credit); cout << " ";
		cout << number << " born"<<endl;
	}
	void icemanwalk() {
		long long declinehp = HP * 1 / 10;
		HP = HP - declinehp;
	}
	void reportsituation() {
		if (color == 0)cout << " red iceman " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else if (color == 1)cout << " blue iceman " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else cout << "wrong color from iceman reportsituation" << endl;
		return;
	}
};

class lion :public warrior {
public:
	long long loyal;

	friend class wolf;
	lion(long long loyal1,long long n,long long hp,long long colorr,long long time,long long agr) {//loyal1 要填写本方司令部的 HP   每一步降低的忠诚度改为外输入参数
		aggressivity = agr;
		credit = 4;
		weapon[sword][1] = 0; weapon[bomb][1] = 0; weapon[arrow][0] = 0; weapon[arrow][1] = 0;//武器初始化
		credit = 4;
		number = n;
		color = colorr;
		HP = hp;
		createweapon(n % 3);
		loyal = loyal1;
		outtime(time); cout << " ";
		outcolor(colorr); cout << " ";
		outprofession(credit); cout << " ";
		cout << number << " born"<<endl;
		cout << "Its loyalty is " << loyal << endl;
	}
	void changeloyal(int K) {
		loyal -= K;
	}
	int getloyal() {
		return loyal;
	}
	void reportsituation() {
		if (color == 0)cout << " red lion " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else if (color == 1)cout << " blue lion " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else cout << "wrong color from lion reportsituation" << endl;
		return;
	}
};

class wolf :public warrior {
public:
	wolf(long long n,long long hp,long long colorr,long long time,long long agr) {
		aggressivity = agr;
		credit = 5;
		weapon[sword][1] = 0; weapon[bomb][1] = 0; weapon[arrow][0] = 0; weapon[arrow][1] = 0;//武器初始化
		HP = hp;
		color = colorr;
		number = n;
		outtime(time); cout << " ";
		outcolor(colorr); cout << " ";
		outprofession(credit); cout << " ";
		cout << number << " born" << endl;
	}
	//参数模版列表主要为getweapon服务的，目的是降低函数重载代码行数，如果使用参数模版列表不能成功运行getweapon函数或者有其他情况，需要将参数化列表改变为多函数重载（参数：类）
	void wolfrobweapon(warrior*dr,long long recordtime) {
		if (dr->credit == 5) return;//对面也是wolf 则不抢夺
	//	cout << dr->number;
	//	cout << " "; outprofession(dr->credit); cout << "local:" << credit;
	//	cout << checkweaponsum()<<"    ";
		long long countweapon = 0;
		countweapon = weapon[sword][1] + weapon[bomb][1] + weapon[arrow][0] + weapon[arrow][1];
		if (countweapon == 10) {
			cout << "already have whole 10 weapon" << endl;
			return;
		}
		else if (countweapon > 10) {
			cout << "wolf" << number << "weapon over 10" << endl;
			return;
		}
		else if (countweapon < 10) {
			dr->updatearrow();
			int leave = 10 - countweapon; int numweap = 0; int symweap = 0;
			int symbolok = 1;//0代表记录已经抢夺过最小的武器了
			while (leave > 0 &&symbolok&& dr->weapon[sword][1] > 0) {
				weapon[sword][1]++;
				dr->weapon[sword][1]--;
				leave--;
				numweap++; symweap = 1;
				if (leave == 0||dr->weapon[sword][1]==0) symbolok = 0;
			}
			while (leave > 0 &&symbolok&& dr->weapon[bomb][1] > 0) {
				weapon[bomb][1]++;
				dr->weapon[bomb][1]--;
				leave--;
				numweap++; symweap = 2;
				if (leave == 0 || dr->weapon[bomb][1] == 0) symbolok = 0;
			}
			//wolf抢夺时将不同耐久的arrow看做是相同序号的，但是耐久为2的优先级更高，所以可以连续抢夺arrow
			while (leave > 0 &&symbolok&& dr->weapon[arrow][1] > 0){
				weapon[arrow][1]++;
				dr->weapon[arrow][1]--;
				leave--;
				numweap++; symweap = 3;
			}
			while (leave > 0 &&symbolok&& dr->weapon[arrow][0] > 0) {
				weapon[arrow][0]++;
				dr->weapon[arrow][0]--;
				leave--;
				numweap++; symweap = 3;
			}
			if (numweap == 0)return;//暂定为如果敌人没有武器，那么不输出任何文字
			else if (symweap == 1) {
				outtime(recordtime);
				if (color == 0) {
					cout << " red wolf " << number << " took " << numweap << " sword from blue ";
					outprofession(dr->credit); cout << " " << dr->number;//in city ?在类外输出
				}
				else if (color == 1) {
					cout << " blue wolf " << number << " took " << numweap << " sword from red ";
					outprofession(dr->credit); cout << " " << dr->number;//in city ?在类外输出
				}
			}
			else if (symweap == 2) {
				outtime(recordtime);
				if (color == 0) {
					cout << " red wolf " << number << " took " << numweap << " bomb from blue ";
					outprofession(dr->credit); cout << " " << dr->number;//in city ?在类外输出
				}
				else if (color == 1) {
					cout << " blue wolf " << number << " took " << numweap << " bomb from red ";
					outprofession(dr->credit); cout << " " << dr->number;//in city ?在类外输出
				}
			}
			else if (symweap == 3) {
				outtime(recordtime);
				if (color == 0) {
					cout << " red wolf " << number << " took " << numweap << " arrow from blue ";
					outprofession(dr->credit); cout << " " << dr->number;//in city ?在类外输出
				}
				else if (color == 1) {
					cout << " blue wolf " << number << " took " << numweap << " arrow from red ";
					outprofession(dr->credit); cout << " " << dr->number;//in city ?在类外输出
				}
			}
		}
		return;
	}
	void reportsituation() {
		if (color == 0)cout << " red wolf " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else if (color == 1)cout << " blue wolf " << number << " has " << weapon[sword][1] << " sword " << weapon[bomb][1] << " bomb " << (weapon[arrow][0] + weapon[arrow][1] + keeparrow) << " arrow and " << HP << " elements" << endl;
		else cout << "wrong color from wolf reportsituation" << endl;
		return;
	}
};

//该函数用于同一个城市中的两方对战,判断A，B是否都存在的语句写在外面   函数功能：实现两个武士的对战，并且直到两房中任一方死亡或者平局，在函数中对两方武士的伤害作用的对象数据，返回void
void battlefight(warrior* A, warrior* B,int checkparity) {
	//A 是输入的红魔军，B是输入的蓝魔军
	if (A == nullptr || B == nullptr) return;//两者有任意一个是空指针，则不进行战斗
	//cout << "redcredit" << A->credit << endl;
	//cout << "redprofession"; outprofession(A->credit);
	
	//cout << "bluecredit" << B->credit << endl;
	//cout << "blueprofession"; outprofession(B->credit);
	A->symfight = 1; B->symfight = 1;//记录双方参与最近一次战斗；
	int checkpt = checkparity;//奇数城市红方先攻击，偶数城市蓝方先攻击
	int keepA = 0; int keepB = 0;//keepA，keepB用于记录排序武器的使用序号
	//long long hpkeepbefore=0; long long hpkeeplast = 1;
	//long long weaponkeepbefore = 0; long long weaponkeeplast = 1; 
	//循环的条件，双方血量或者武器能发生变化且双方血量都大于零
	int firstfightA = 0; int firstfightB = 0;
	//A->weaponsum = A->weapon[sword][1] + A->weapon[bomb][1] + A->weapon[arrow][0] + A->weapon[arrow][1]; B->weaponsum = B->weapon[sword][1] + B->weapon[bomb][1] + B->weapon[arrow][0] + B->weapon[arrow][1];
	while (A->HP>0 && B->HP>0 && (((A->weapon[bomb][1]+B->weapon[bomb][1]+A->weapon[arrow][0]+B->weapon[arrow][0]+A->weapon[arrow][1]+B->weapon[arrow][1]+A->keeparrow+B->keeparrow)>0)||((A->weapon[sword][1]>0&&(A->aggressivity*2/10>0))||B->weapon[sword][1]>0&&(B->aggressivity*2/10>0)))) {//(hpkeepbefore!=hpkeeplast||weaponkeepbefore!=weaponkeeplast)
		
		int calc1 = 0; int calc2 = 0;
		if (keepA==A->weaponsum||firstfightA==0) {
			A->updatearrow();//表示给红魔军更新箭支
			A->countweapon();
			firstfightA = 1;
			keepA = 0;
		}
		if (keepB==B->weaponsum||firstfightB==0) {
			B->updatearrow();//表示给蓝魔军更新箭支
			B->countweapon();
			firstfightB = 1;
			keepB = 0;
		}
		if (checkpt % 2 == 1) {
			//判断有无武器,如果没有，那么直接下一个循环(因为在A使用完武器后，没有进行weaponline中的更新0赋值)
			if (A->checkweaponsum() <= 0) {
				if (A->checkweaponsum() < 0) cout << "Wrong weaponnum <0 in credit: " << A->credit << " num " << A->number << endl;//武器数目小于0则报错
				checkpt++; continue;
			}
			if (A->weaponline[keepA] == 1) {
				B->gethurt(A->usesword());
				keepA++;
			}
			else if (A->weaponline[keepA] == 2) {
				B->gethurt(A->usebomb());
				keepA++;
			}
			else if (A->weaponline[keepA] == 3) {
				B->gethurt(A->usearrow0());
				keepA++;
			}
			else if (A->weaponline[keepA] == 4) {
				B->gethurt(A->usearrow1());
				keepA++;
			}
		}
		else {
			if (B->checkweaponsum() <= 0) {
				if (B->checkweaponsum() < 0) cout << "Wrong weaponnum<0 in credit:" << B->credit << " num " << B->number << endl;//武器数目小于0则报错
				checkpt++; continue;
			}
			if (B->weaponline[keepB] == 1) {
				A->gethurt(B->usesword());
				keepB++;
			}
			else if (B->weaponline[keepB] == 2) {
				A->gethurt(B->usebomb());
				keepB++;
			}
			else if (B->weaponline[keepB] == 3) {
				A->gethurt(B->usearrow0());
				keepB++;
			}
			else if (B->weaponline[keepB] == 4) {
				A->gethurt(B->usearrow1());
				keepB++;
			}
		}
		checkpt++;
	//	hpkeeplast = A->HP + B->HP;
	//	weaponkeepbefore = A->checkweaponsum() + B->checkweaponsum();
	}
}

void outcitysituation(warrior* A, warrior* B,long long recordtime,int citynum) {
	//A代表红魔军，B代表蓝魔军
	if (A == nullptr || B == nullptr) return;//同一个城市没有两个以上的武士，则不发起战斗，所以暂定为不汇报战斗情况
	//红魔军战胜蓝魔军
	if (A->HP > 0 && B->HP <= 0) {
		outtime(recordtime); cout << " red ";
		outprofession(A->credit);
		cout << " " << A->number << " killed blue ";
		outprofession(B->credit);
		cout << " " << B->number<<" in city "<<citynum<<" remaining "<<A->HP<<" elements"<<endl;
	}
	else if (A->HP <= 0 && B->HP > 0) {
		outtime(recordtime); cout << " blue ";
		outprofession(B->credit);
		cout << " " << B->number<<" killed red ";
		outprofession(A->credit);
		cout << " " << A->number << " in city " << citynum << " remaining " << B->HP << " elements"<<endl;
	}
	else if (A->HP > 0 && B->HP > 0) {
		outtime(recordtime);
		cout << " both red ";
		outprofession(A->credit);
		cout << " " << A->number;
		cout << " and blue ";
		outprofession(B->credit);
		cout << " " << B->number << " were alive in city " << citynum << endl;
	}
	else if (A->HP <= 0 && B->HP <= 0) {
		outtime(recordtime);
		cout << " both red ";
		outprofession(A->credit);
		cout << " " << A->number;
		cout << " and blue ";
		outprofession(B->credit);
		cout << " " << B->number << " died in city " << citynum << endl;
	}
}













#endif // _WARRIOR_