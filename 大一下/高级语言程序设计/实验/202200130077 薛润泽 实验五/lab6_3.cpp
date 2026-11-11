#include<iostream>

using namespace std;

char a[1000];
char b[1000];
int main(){
	scanf("%s",a);
	scanf("%s",b);
	int couta=0;
	while(a[couta]!=NULL) couta++;
	int coutb=0;
	while(b[coutb]!=NULL){
		a[couta++]=b[coutb++];
	}
	cout<<a<<endl;
	return 0;
}
