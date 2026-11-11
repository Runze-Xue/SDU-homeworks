// P163 32题 
#include<iostream>

int main(){
	int R1, C1, R2, C2;//分别记录上、下三角矩阵的行数和列数 
	std::cin>>R1>>C1>>R2>>C2;
	if(C1 != R1 || C2 != R2){
		std::cout<<"不是一个方阵";
		return 0;
	}
	if(R1 != C2){
		std::cout<<"无法完成矩阵乘法";
		return 0;
	} 
	int *UTM = new int[C1*(C1 + 1)/2 + 1]; 
	int *LTM = new int[C2*(C2 + 1)/2 + 1];
	for(int i = 1; i <= C1*(C1 + 1)/2; i++){
		std::cin>>UTM[i]; 
	} //自左向右，自上向下输入上三角矩阵 
	for(int i = 1; i <= C2*(C2 + 1)/2; i++){
		std::cin>>LTM[i];
	} //自左向右，自上向下输入下三角矩阵 
	int **result = new int*[C1 + 1];
	for(int i = 1; i <= C1; i++){
		result[i] = new int[R2 + 1];
	}
	int ElmentNum = C1*(C1 + 1)/2; //记录两个矩阵的有意义元素个数 
	for(int i = 1; i <= C1; i++){//i是上三角矩阵的行
		for(int j = 1; j <= R2; j++){//j是下三角矩阵的列 
			int cntans = 0;
			for(int cnt = 1; cnt <= C1; cnt++){//对于i，cnt是列数，对于j，cnt是行数 
				if(i > cnt || j > cnt) ;//因为加零，所以不执行任何操作
				else {
					cntans += UTM[ElmentNum - (C1 - i + 1)*(C1 - i + 2)/2 + cnt - i + 1]* LTM[(cnt-1)*cnt/2 + j];//算出相应位置的矩阵元素乘积，并求加和 
				} 
			}
			result[i][j] = cntans; //将算完的结果赋值到相应位置上 
		} 
	} 
	for(int i = 1; i <= C1; i++){
		for(int j = 1; j <= C1; j++){
			std::cout<<result[i][j]<<' ';
		}
		std::cout<<'\n';
	}
	return 0;
}
