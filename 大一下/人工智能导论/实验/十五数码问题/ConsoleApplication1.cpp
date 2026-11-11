#include<iostream>//万能头文件
#include<algorithm>
#include<math.h>
#include<cmath>
#include<queue>
#include<windows.h>//含有计算运行时间的函数 
#define int unsigned long long
using namespace std;

//gx()定义为从根节点到x结点的代价总和，每次都移动一步，所以恒定为1 
//int gx(){
//hx()定义为从x节点到目标结点的代价总和，使用曼哈顿距离 
//fx=gx+hx=hx，所以直接使用hx
int findval(int cntsym, int aim) {
	//	return 1;//当估值函数恒为1，即使用广度优先搜索时使用 
	int cntsymban = cntsym; int aimban = aim;
	int a1[5][5];
	int a2[5][5];
	int k = 0x1000000000000000;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			a1[i][j] = cntsymban / k;
			cntsymban = cntsymban - cntsymban / k * k;
			k = k / 0x10;
		}
	}

	k = 0x1000000000000000;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			a2[i][j] = aimban / k;
			aimban = aimban - aimban / k * k;
			k = k / 0x10;
		}
	}
	int sv = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (int ii = 1; ii <= 4; ii++) {
				for (int jj = 1; jj <= 4; jj++) {
					if (a1[i][j] == a2[ii][jj]) {
						sv += fabs(i - ii) + fabs(j - jj);
					}
				}
			}
		}
	}
	return  sv;
}
//上述撰写估值函数；
const int movedis[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
int move(int cntsym, int dir) {
	
	//	cout<<"dir"<<dir<<endl;
	//	cout<<"move前："<<hex<<cntsym<<endl;
	int cntsymban = cntsym;
	int a1[5][5];
	int k = 0x1000000000000000;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			a1[i][j] = cntsymban / k;
			cntsymban = cntsymban - cntsymban / k * k;
			k = k / 0x10;
		}
	}
	//asdfasdg
	int sym2 = 0;
	for (int i = 1; i <= 4; i++) {
		if (sym2 == 1) break;
		for (int j = 1; j <= 4; j++) {
			if (sym2 == 1) break;
			//寻找 0 点位置 
			if (a1[i][j] == 0) {
				if ((i + movedis[dir][0] > 4) || (i + movedis[dir][0] < 1 )|| (j + movedis[dir][1]>4 )||( j + movedis[dir][1] < 1)) {
							cout<<"跳过了"<<endl;
					return 0;
					}
				int bann = a1[i + movedis[dir][0]][j + movedis[dir][1]];
				a1[i + movedis[dir][0]][j + movedis[dir][1]] = a1[i][j];
				a1[i][j] = bann;
				sym2 = 1;
			}
		}
	}
	int k1 = 1; int sum = 0;
	for (int i = 4; i >= 1; i--) {
		for (int j = 4; j >= 1; j--) {
			sum += a1[i][j] * k1; k1 = k1 * 0x10;
		}
	}
	//	cout<<"move后"<<hex<<sum<<endl;
	return sum;
}
//上述撰写移动函数 
struct number {
	int hx;
	int cnt;
	int step;
	int nots;
};
bool operator<(const number& a, const number& b) {
	return a.hx > b.hx;
}
//上述撰写结构体与重载排序 
struct number nm;
int mem[100000]; int memoloc = 2; int sumnot = 1;
signed main() {
	unsigned long long cntsym; unsigned long long aim;
	//scanf("%0x%0x",&cntsym,&aim);
	cin >> hex >> cntsym >> aim;
	priority_queue<number>q;//把第一个输入优先队列 
	int val1 = findval(cntsym, aim);
	nm.hx = val1; nm.cnt = cntsym; nm.nots = 1;
	mem[1] = nm.cnt; mem[0] = 0;
	q.push(nm);
	//q.top().cnt!=aim&&!q.empty()
	DWORD time_start, time_end;
	time_start = GetTickCount64();
	//    q.top().cnt!=aim&&!q.empty() 
	int qt = 1;
	while (qt >= 0) {
		nm = q.top();
		struct number num1;
		q.pop();
		for (int i = 0; i <= 3; i++) {
			if (move(nm.cnt, i) != 0) {
				int mv = move(nm.cnt, i);
				cout << hex << move(nm.cnt, i) << endl;
				int   mban = lower_bound(mem, mem + memoloc, mv) - mem;//试试 
				if (mem[mban] != move(nm.cnt, i)) {
					mem[memoloc] = move(nm.cnt, i);
					memoloc++;
					sort(mem + 1, mem + memoloc);
					num1.cnt = move(nm.cnt, i);
					num1.hx = findval(move(nm.cnt, i), aim);
					sumnot++;
					num1.step = nm.step + 1;
					num1.nots = sumnot;
					q.push(num1);
					//		cout<<"q.push:"<<hex<<q.top().cnt<<endl;
				}
				else continue;
			}
		}
		qt--;
	}
	cout << "步数:" << q.top().step << endl;
	cout << "结点数:" << q.top().nots << endl;
	time_end = GetTickCount64();
	cout << "Time = " << (time_end - time_start) << "ms\n ";
	system("pause");
	return 0;
}