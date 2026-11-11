#include<bits/stdc++.h>
using namespace std;
//gx()定义为从根节点到x结点的代价总和，每次都移动一步，所以恒定为1 
//int gx(){
//hx()定义为从x节点到目标结点的代价总和，使用曼哈顿距离 
//fx=gx+hx=hx，所以直接使用hx
int memory[1000000];
int memorycount=0;//用来记忆已经找过的状态 
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
int motionleft(int cntsym){
	int cntsymban=cntsym;
	int a1[4][4];
	for(int i=3;i>=1;i--){
		for(int j=3;j>=1;j--){
		a1[i][j]=cntsymban-cntsymban/10;
		cntsymban=cntsymban/10;
		if(a1[i][j]==0){
		  if(j==1) return 0;
		  else {
			int ban1=a1[i][j-1];
			a1[i][j-1]=0;
			a1[i][j]=ban1;
			int k=1;int rtsum=0;
		  for(int ii=3;ii>=1;ii--){
			for(int jj=3;jj>=1;jj--){
				rtsum+=a1[ii][jj]*k;
				k=k*10;
			}
		}
		return rtsum;
		}
	    	}
		}
	}
}
int motionright(int cntsym){
	int cntsymban=cntsym;
	int a1[4][4];
	for(int i=3;i>=1;i--){
		for(int j=3;j>=1;j--){
		a1[i][j]=cntsymban-cntsymban/10;
		cntsymban=cntsymban/10;
		if(a1[i][j]==0){
		  if(j==3) return 0;
		  else {
			int ban1=a1[i][j+1];
			a1[i][j+1]=0;
			a1[i][j]=ban1;
			int k=1;int rtsum=0;
		  for(int ii=3;ii>=1;ii--){
			for(int jj=3;jj>=1;jj--){
				rtsum+=a1[ii][jj]*k;
				k=k*10;
			}
		}
		return rtsum;
		}
	    	}
		}
	}
}
int motionup(int cntsym){
	int cntsymban=cntsym;
	int a1[4][4];
	for(int i=3;i>=1;i--){
		for(int j=3;j>=1;j--){
		a1[i][j]=cntsymban-cntsymban/10;
		cntsymban=cntsymban/10;
		if(a1[i][j]==0){
		  if(i==1) return 0;
		  else {
			int ban1=a1[i-1][j];
			a1[i-1][j]=0;
			a1[i][j]=ban1;
			int k=1;int rtsum=0;
		  for(int ii=3;ii>=1;ii--){
			for(int jj=3;jj>=1;jj--){
				rtsum+=a1[ii][jj]*k;
				k=k*10;
			}
		}
		return rtsum;
		}
	    	}
		}
	}
}
int motiondown(int cntsym){
	int cntsymban=cntsym;
	int a1[4][4];
	for(int i=3;i>=1;i--){
		for(int j=3;j>=1;j--){
		a1[i][j]=cntsymban-cntsymban/10;
		cntsymban=cntsymban/10;
		if(a1[i][j]==0){
		  if(i==3) return 0;
		  else {
			int ban1=a1[i+1][j];
			a1[i+1][j]=0;
			a1[i][j]=ban1;
			int k=1;int rtsum=0;
		  for(int ii=3;ii>=1;ii--){
			for(int jj=3;jj>=1;jj--){
				rtsum+=a1[ii][jj]*k;
				k=k*10;
			}
		}
		return rtsum;
		}
	    	}
		}
	}
}
//上述撰写四个移动函数； 
struct number{
	int hx;
	int cnt;
};
bool operator<(const number &a,const number &b){
		return a.hx <= b.hx;
	}

int step=0;
struct number nm;
int main(){
	int cntsym=0;int aim=0;
	scanf("%d%d",&cntsym,&aim);
	priority_queue<number> q;
	int val1=findval(cntsym,aim); 
	nm.hx=val1; nm.cnt=cntsym;
	q.push(nm);
	
	while(q.top().hx!=0){
		
		nm=q.top();
		number num1;
		q.pop();
		if(motionright(nm.cnt)!=0){
			num1.cnt=motionright(nm.cnt);
			num1.hx=findval(num1.cnt,aim);
			q.push(num1);
		}
		
		if(motionleft(nm.cnt)!=0){
			num1.cnt=motionleft(nm.cnt);
			num1.hx=findval(num1.cnt,aim);
		q.push(num1);
		}
		if(motionup(nm.cnt)!=0){
			num1.cnt=motionup(nm.cnt);
			num1.hx=findval(num1.cnt,aim);
			q.push(num1);
		}
		if(motiondown(nm.cnt)!=0){
			num1.cnt=motiondown(nm.cnt);
			num1.hx=findval(num1.cnt,aim);
			q.push(num1);
		}
		step++;
		
	}
	cout<<step;
	return 0;
}
