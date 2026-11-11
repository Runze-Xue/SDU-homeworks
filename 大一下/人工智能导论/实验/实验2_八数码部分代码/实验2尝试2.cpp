#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
//gx()定义为从根节点到x结点的代价总和，每次都移动一步，所以恒定为1 
//int gx(){
//hx()定义为从x节点到目标结点的代价总和，使用曼哈顿距离 
//fx=gx+hx=hx，所以直接使用hx
int findval(int cntsym,int aim){
	int cntsymban=cntsym; int aimban=aim;
	int a1[4][4];
	int a2[4][4];
	int k=100000000;
	for(int i=1;i<=3;i++){
		for(int j=1;j<=3;j++){
		a1[i][j]=cntsymban/k;
		cntsymban=cntsymban-cntsymban/k*k;
		k=k/10;
		}
	}
	k=100000000;
	for(int i=1;i<=3;i++){
		for(int j=1;j<=3;j++){
		a2[i][j]=aimban/k;
		aimban=aimban-aimban/k*k;
		k=k/10;
		}
	}
	int sv = 0;
	for(int i = 1; i<=3; i++){
		for(int j = 1 ;j<=3 ;j++){
			for(int ii = 1;ii<=3;ii++){
				for(int jj = 1; jj<=3 ;jj++){
					if(a1[i][j]==a2[ii][jj]){
						sv+=abs(i-ii)+abs(j-jj);
					}
				}
			}
		}
	}
	return  sv;
}
//上述撰写估值函数；
const int movedis[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
int move(int cntsym,int dir){
	int cntsymban=cntsym;
	int a1[4][4];
	int k=100000000;
	for(int i=1;i<=3;i++){
		for(int j=1;j<=3;j++){
		a1[i][j]=cntsymban/k;
		cntsymban=cntsymban-cntsymban/k*k;
		k=k/10;
		}
	}
	//asdfasdg
	int sym2=0;
	for(int i=1;i<=3;i++){
		if(sym2==1) break;
		for(int j=1;j<=3;j++){
			if(sym2==1) break;
		if(a1[i][j]==0){
//			cout<<"坐标为："<<i<<' '<<j<<endl;
			if(i+movedis[dir][0]>3||i+movedis[dir][0]<1||j+movedis[dir][1]>3||j+movedis[dir][1]<1) return 0;
		int bann=a1[i+movedis[dir][0]][j+movedis[dir][1]];
		a1[i+movedis[dir][0]][j+movedis[dir][1]]=a1[i][j];
		a1[i][j]=bann;
//		cout<<"坐标为："<<i+movedis[dir][0]<<' '<<j+movedis[dir][1]<<endl;
		sym2=1;
	    	}
		}
	}
	int k1=1;int sum=0;
	for(int i=3;i>=1;i--){
		for(int j=3;j>=1;j--){
			sum+=a1[i][j]*k1;k1=k1*10;
		}
	}
	return sum;
}
//上述撰写移动函数 
struct number{
	int hx;
	int cnt;
	int step;
	int nots;
};
bool operator<(const number &a,const number &b){
		return a.hx >= b.hx;
	}
//上述撰写结构体与重载排序 
struct number nm;
int mem[100000]; int memoloc=2; int sumnot=1;
int main(){
	int cntsym=0;int aim=0;
	scanf("%d%d",&cntsym,&aim);
	priority_queue<number> q;//把第一个输入优先队列 
	int val1=findval(cntsym,aim); 
	nm.hx=val1; nm.cnt=cntsym; nm.nots=1;
	mem[1]=nm.cnt;mem[0]=0;
	q.push(nm);
	//q.top().cnt!=aim&&!q.empty()
			DWORD time_start, time_end;
    time_start = GetTickCount();
    //q.top().cnt!=aim&&!q.empty()   qt>=0
	while(q.top().cnt!=aim&&!q.empty()){
		nm=q.top();
		 struct number num1;
		q.pop();
		for(int i=0;i<=3;i++){
			if(move(nm.cnt,i)!=0){
				int mv=move(nm.cnt,i);
			int   mban= lower_bound(mem,mem+memoloc,mv)-mem;//试试 
			if(mem[mban]!=move(nm.cnt,i)){
				mem[memoloc]=move(nm.cnt,i);
				memoloc++;
				sort(mem+1,mem+memoloc);
				num1.cnt=move(nm.cnt,i);
				num1.hx=findval(move(nm.cnt,i),aim);
				sumnot++;
				num1.step=nm.step+1;
				num1.nots=sumnot;
				q.push(num1); 
			}
			else continue;
	    	}
		}
	}
 	 cout<<"步数:"<<q.top().step<<endl;
 	 cout<<"结点数:"<<q.top().nots<<endl; 
	time_end = GetTickCount();
    cout << "Time = " << (time_end - time_start) << "ms\n ";
	system("pause");
	return 0;
}
