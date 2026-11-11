#include<iostream>

using namespace std;

void change(int (*a)[3],int(*ans)[3]){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			*(*(ans+j)+i)=*(*(a+i)+j);
		}
	}
}
int (*a)[3]=new int[3][3];
int (*ans)[3]=new int[3][3];
int main(){ 
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
         scanf("%d",*(a+i)+j);

		}
	}
	change(a,ans);
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			printf("%d ",*(*(ans+i)+j));
		}
		printf("\n"); 
	}
	delete []a;delete []ans;
	return 0;
}

//a b c
//d e f
//i j k 

//a d i
//b e j
//c f k
