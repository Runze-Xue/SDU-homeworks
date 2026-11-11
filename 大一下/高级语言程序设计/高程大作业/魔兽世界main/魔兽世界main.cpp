#include<iostream>
#include"warrior.h"

using namespace std;

long long recordtime=0;//记录全局时间流向
int t;//组数
long long redselection = 0;//记录武士生成到哪一个了
long long blueselection = 0;
long long rednumber = 1;
long long bluenumber = 1;
long long redHP;//记录红方基地生命值
long long blueHP;
long long dragonHP;
long long ninjiaHP;
long long icemanHP;
long long lionHP;
long long wolfHP;
long long dragonagr, ninjiaagr, icemanagr, lionagr, wolfagr;
int enoughhp = 1;
int enoughhp2 = 1;
warrior *citysets[30][2];//第一维存所在城市，第二维存所在阵营，0是红魔军，1是蓝魔军，

//从左到右依次是：红司令部，City 1，City 2，……，City n，蓝司令部

//没有city类，用数组来代表city，数组的第一个元素和最后一个元素代表总部

//函数用来生成武士，createredarmy生成红色军队
warrior* createredarmy() {
	int color = 0;
	long long *colorHP=nullptr;	long long *colornumber=nullptr; long long *selection=nullptr;
		colorHP = &redHP;
		colornumber = &rednumber;
		selection = &redselection;

	long long choice = (*selection) % 5;

	if (choice == 0&&enoughhp) {
		if ((*colorHP) < icemanHP) {
			enoughhp = 0; (*selection)++;
			return nullptr;
		}
		else {
			(*colorHP) -= icemanHP;
			warrior* soldier = new iceman((*colornumber), icemanHP, 0, recordtime,icemanagr);
			(*colornumber)++; (*selection)++;
			return soldier;
		}
	}
	choice = (*selection) % 5;
	 if (choice == 1&&enoughhp) {
		 if ((*colorHP) < lionHP) {
			 (*selection)++;
			 enoughhp = 0;
			 return nullptr;
		 }
		 else {
			 (*colorHP) -= lionHP;
			 warrior* soldier = new lion((*colorHP),(*colornumber),  lionHP, 0, recordtime,lionagr);
			 (*colornumber)++; (*selection)++;
			 return soldier;
		 }
	}
	 choice = (*selection) % 5;
	 if (choice == 2&&enoughhp) {
		 if ((*colorHP) < wolfHP) {
			 (*selection)++;
			 enoughhp = 0;
			 return nullptr;
		 }
		 else {
			 (*colorHP) -= wolfHP;
			 warrior* soldier = new wolf((*colornumber),wolfHP,0, recordtime,wolfagr);
			 (*colornumber)++; (*selection)++;
			 return soldier;
		 }
	}
	 choice = (*selection) % 5;
	 if (choice == 3&&enoughhp) {
		 if ((*colorHP) < ninjiaHP) {
			 (*selection)++;
			 enoughhp = 0;
			 return nullptr;
		 }
		 else {
			 (*colorHP) -= ninjiaHP;
			 warrior* soldier = new ninjia((*colornumber), ninjiaHP, 0,recordtime,ninjiaagr);
			 (*colornumber)++; (*selection)++;
			 return soldier;
		 }
	}
	 choice = (*selection) % 5;
	 if (choice == 4&&enoughhp ) {
		 if ((*colorHP) < dragonHP) {
			 (*selection)++;
			 enoughhp = 0;
			 return nullptr;
		 }
		 else {
			 (*colorHP) -= dragonHP;
			 warrior* soldier = new dragon((*colornumber), dragonHP, 0, recordtime,dragonagr);
			 (*colornumber)++; (*selection)++;
			 return soldier;
		 }
	}
	 //如果一个也不合适，那么返回空指针
	 return nullptr;
}
//createbluearmy 生成蓝色军队
warrior* createbluearmy() {
	int color = 1;
	long long* colorHP=nullptr;	long long* colornumber=nullptr; long long* selection=nullptr;
	if (color == 0) {
		colorHP = &redHP;
		colornumber = &rednumber;
		selection = &redselection;
	}
	else if (color == 1) {
		colorHP = &blueHP;
		colornumber = &bluenumber;
		selection = &blueselection;
	}
	else cout << "Wong Color" << endl;

	long long choice = (*selection) % 5;
	if (choice == 0&&enoughhp2) {
		if ((*colorHP) < lionHP) {
			(*selection)++;
			enoughhp2 = 0;
			return nullptr;
		}
		else {
			(*colorHP) -= lionHP;
			warrior* soldier = new lion((*colorHP),(*colornumber), lionHP, 1, recordtime,lionagr);
			(*colornumber)++; (*selection)++;
			return soldier;
		}
	}
	choice = (*selection) % 5;
	if (choice == 1&&enoughhp2) {
		if ((*colorHP) < dragonHP) {
			(*selection)++;
			enoughhp2 = 0;
			return nullptr;
		}
		else {
			(*colorHP) -= dragonHP;
			warrior* soldier = new dragon((*colornumber), dragonHP, 1, recordtime,dragonagr);
			(*colornumber)++; (*selection)++;
			return soldier;
		}
	}
	choice = (*selection) % 5;
	if (choice == 2&&enoughhp2) {
		if ((*colorHP) < ninjiaHP) {
			(*selection)++;
			enoughhp2 = 0;
			return nullptr;
		}
		else {
			(*colorHP) -= ninjiaHP;
			warrior* soldier = new ninjia((*colornumber), ninjiaHP, 1, recordtime,ninjiaagr);
			(*colornumber)++; (*selection)++;
			return soldier;
		}
	}
	choice = (*selection) % 5;
	if (choice == 3&&enoughhp2) {
		if ((*colorHP) < icemanHP) {
			(*selection)++;
			enoughhp2 = 0;
			return nullptr;
		}
		else {
			(*colorHP) -= icemanHP;
			warrior* soldier = new iceman((*colornumber), icemanHP, 1, recordtime,icemanagr);
			(*colornumber)++; (*selection)++;
			return soldier;
		}
	}
	choice = (*selection) % 5;
	if (choice == 4&&enoughhp2) {
		if ((*colorHP) < wolfHP) {
			(*selection)++;
			enoughhp2 = 0;
			return nullptr;
		}
		else {
			(*colorHP) -= wolfHP;
			warrior* soldier = new wolf((*colornumber),wolfHP,1, recordtime,wolfagr);
			(*colornumber)++; (*selection)++;
			return soldier;
		}
	}
	//如果一个也不合适，那么返回空指针
	return nullptr;
}

//主函数开始
int main() {
	cin >> t;
	for (int tt = 1; tt <= t; tt++) {
		recordtime = 0;
		redselection = 0;//记录武士生成到哪一个了
		blueselection = 0;
		rednumber = 1;
		bluenumber = 1;
		int M, N, K,T;
		cin >> M >> N >> K >> T;//T是截止时间，以分钟为单位
		//N是两个司令部相隔的城市树木,所以红魔军司令部为 0 ，蓝魔军司令部位 N+1
		//M是司令部的生命元
		redHP = M; blueHP = M;
		cin >> dragonHP >> ninjiaHP >> icemanHP >> lionHP >> wolfHP;
		cin >> dragonagr >> ninjiaagr >> icemanagr >> lionagr >> wolfagr;
		enoughhp = 1; enoughhp2 = 1;
		cout << "Case " << tt << ":" << endl;

		//清空指针数组
		for (int i = 0; i <= 29; i++) {
			for (int j = 0; j <= 1; j++) {
				if (citysets[i][j] != nullptr && (citysets[i][j]->credit==1||citysets[i][j]->credit==2||citysets[i][j]->credit==3||citysets[i][j]->credit==4||citysets[i][j]->credit==5)) {
					delete citysets[i][j];
				}
				citysets[i][j] = nullptr;
			}
		}
		while (recordtime<=T) {
			//注意数组的第0位是左侧红魔军司令部，第 N+1 位是蓝魔军司令部  数组warrior *citysets[30][2]; 函数create?army()
			//citysets[30][2]中用于存放warrior类型指针,其中[][0]存放红魔军队的指针，[][1]存放蓝魔军队的指针
			citysets[0][0] = createredarmy();
			citysets[N + 1][1] = createbluearmy();

		//	cout << "red new warriror" << citysets[0][0]->number<<citysets[0][0]->credit << endl;
		//	cout << "blue new warrior" << citysets[N + 1][1]->number << citysets[N + 1][1]->credit << endl;
			if (T < recordtime + 5) break;
			// 时间增加 5min 并且检测逃跑的lion ，释放逃跑的lion
			recordtime += 5;
			for (int i = 1; i <= N; i++) {
				for (int j = 0; j <= 1; j++) {
					if (citysets[i][j] != nullptr && citysets[i][j]->credit == 4) {//citysets[i][j] != nullptr && citysets[i][j]->credit == 4
						if (citysets[i][j]->getloyal() <= 0) {
							outtime(recordtime); 
							if (j == 0)cout << " red lion ";
							else if (j == 1)cout << " blue lion ";
							cout << citysets[i][j]->number;
							cout << " ran away"<<endl;
							delete citysets[i][j];
							citysets[i][j] = nullptr;
						}
					}
				}
			}
			if (T < recordtime + 5) break;
			recordtime += 5;
			//红魔军整体移动
			//cout << "fiste" << endl;
			for (int i = N + 1; i >= 1; i--) {
				citysets[i][0] = citysets[i - 1][0];
				if (citysets[i][0] != nullptr) {
					citysets[i][0]->icemanwalk();
					citysets[i][0]->changeloyal(K);
					citysets[i - 1][0] = nullptr;
				}
			}//蓝魔军整体移动
			for (int i = 0; i <= N; i++) {
				citysets[i][1] = citysets[i + 1][1];
				if (citysets[i][1] != nullptr) {
					citysets[i][1]->icemanwalk();
					citysets[i][1]->changeloyal(K);
					citysets[i + 1][1] = nullptr;
				}
			}

			//武士前进到某一城市
			for (int i = 0; i <= N+1; i++) {
				for (int j = 0; j <= 1; j++) {
					if (i == 0) {
						if (j==1&&citysets[0][1] != nullptr) {
							outtime(recordtime); cout << " blue ";
							outprofession(citysets[0][1]->credit);
							cout << " " << citysets[0][1]->number << " reached red headquarter with " << citysets[0][1]->HP << " elements and force " << citysets[0][1]->aggressivity << endl;
							outtime(recordtime); cout << " red headquarter was taken" << endl;
						}
						continue;
					}
					if (i == N + 1) {
						if (j==0&&citysets[N + 1][0] != nullptr) {
							outtime(recordtime); cout << " red ";
							outprofession(citysets[N + 1][0]->credit);
							cout << " " << citysets[N + 1][0]->number << " reached blue headquarter with " << citysets[N + 1][0]->HP << " elements and force " << citysets[N + 1][0]->aggressivity << endl;
							outtime(recordtime); cout << " blue headquarter was taken" << endl;
						}
						continue;
					}
					if (citysets[i][j] == nullptr) {
						//cout <<"i"<<i <<"j" <<j<< citysets << endl;
						continue;
					}
					outtime(recordtime);
					if (j == 0)cout << " red ";
					else if (j == 1)cout << " blue ";
					outprofession(citysets[i][j]->credit);
					cout << " "; cout << citysets[i][j]->number;
					cout << " marched to city " << i << " with " << citysets[i][j]->HP << " elements and force " << citysets[i][j]->aggressivity << endl;
				}
			}
			//判断是否占领司令部
			if (citysets[0][1] != nullptr || citysets[N + 1][0] != nullptr) {
				recordtime = 0; break;
			}
			//wolf 抢夺武器
			if (T < recordtime + 25) break;
			recordtime += 25;
				for (int i = 1; i <= N; i++) {
					if (citysets[i][0] == nullptr || citysets[i][1] == nullptr) continue;
					if ((citysets[i][1]->checkweaponsum() > 0) && citysets[i][0]->checkweaponsum() != 10 && (citysets[i][1]->credit != citysets[i][0]->credit) && (citysets[i][0]->credit == 5))
					{
						citysets[i][0]->wolfrobweapon(citysets[i][1], recordtime);//红魔抢夺蓝魔
						cout << " in city " << i << endl;
					}
					if ((citysets[i][0]->checkweaponsum() > 0) && (citysets[i][1]->checkweaponsum() != 10) && (citysets[i][1]->credit != citysets[i][0]->credit) && (citysets[i][1]->credit == 5)) 
					{
						citysets[i][1]->wolfrobweapon(citysets[i][0], recordtime);//蓝魔抢夺红魔
						cout << " in city " << i << endl; }
				}
			//wolf 偷盗完毕
			//开始对战
			if (T < recordtime + 5)break;
			recordtime += 5;
				for (int i = 1; i <= N; i++) {
				battlefight(citysets[i][0], citysets[i][1], i);
				if (citysets[i][1]!=nullptr&&citysets[i][0] != nullptr&& citysets[i][0]->number == 6) {
				}
			}
				//武器抢夺
				for (int i = 1; i <= N; i++) {
					if (citysets[i][0] == nullptr || citysets[i][1] == nullptr) continue;
					if (citysets[i][0]->HP > 0 && citysets[i][1]->HP <= 0) {
						citysets[i][0]->getweapon(citysets[i][1]);
					}
					else if (citysets[i][0]->HP <= 0 && citysets[i][1]->HP > 0) {
						citysets[i][1]->getweapon(citysets[i][0]);
					}
				}
				//报告战斗结果
				for (int i = 1; i <= N; i++) {
					outcitysituation(citysets[i][0], citysets[i][1], recordtime, i);
					if (citysets[i][0] != nullptr && citysets[i][0]->credit == 1 && citysets[i][0]->HP > 0 && citysets[i][0]->symfight == 1) {
						outtime(recordtime); cout << " red dragon " << citysets[i][0]->number << " yelled in city " << i << endl;
						citysets[i][0]->symfight = 0;
					}
					if (citysets[i][1] != nullptr && citysets[i][1]->credit == 1 && citysets[i][1]->HP > 0 && citysets[i][1]->symfight == 1) {
						outtime(recordtime); cout << " blue dragon " << citysets[i][1]->number << " yelled in city " << i << endl;
						citysets[i][1]->symfight = 0;
					}
				}
				//归零死亡武士
				for (int ii = 1; ii <= N; ii++) {
					for (int jj = 0; jj <= 1; jj++) {
						if (citysets[ii][jj] != nullptr) {
							if (citysets[ii][jj]->HP <= 0) {
								delete citysets[ii][jj];
								citysets[ii][jj] = nullptr;
							}
						}
					}
				}


			if (T < recordtime + 10) break;
			recordtime += 10;
				//司令部报告生命元
			outtime(recordtime); cout << " " << redHP << " elements in red headquarter" << endl;
			outtime(recordtime); cout << " " << blueHP << " elements in blue headquarter " << endl;
				//武士报告武器拥有情况
			if (T < recordtime + 5) break;
			recordtime += 5;
			for (int i = 1; i <= N; i++) {
				//红蓝双方司令部没有考虑，因为一定会经过移动，司令部中武士一定移动出司令部
				if (citysets[i][0] != nullptr) {
					outtime(recordtime); citysets[i][0]->reportsituation();
				}
				if (citysets[i][1] != nullptr) {
					outtime(recordtime); citysets[i][1]->reportsituation();
				}
			}
			if (T < recordtime + 5) break;
			recordtime += 5;
		}
	}
	return 0;
}