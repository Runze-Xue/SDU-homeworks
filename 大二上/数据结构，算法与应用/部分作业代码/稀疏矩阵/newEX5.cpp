#include<iostream>
int Read(){
	int x = 0, f = 1;
	char c = getchar();
	while(c < '0' || c > '9'){
		if(c == '-'){
			f = -1;
		}
		c = getchar();
	}
	while(c >= '0' && c <= '9'){
		x = x*10+c-'0';
		c = getchar();
	}
	return x*f;
}

struct matrixTerm{
	int row = -1;//判断未被初始化 
	int col = -1;
	int value;
	matrixTerm& operator=(const matrixTerm& A){
		this->row = A.row;
		this->col = A.col;
		this->value = A.value;
		return *this;
	}
};

class spareMatrix{
public:
	void resetMatrix(){//重置数组 
		int n, m;
		realnum = 0;
		n = Read(); m = Read();
		delete []element;
		element = new matrixTerm[n*m + 1];//最多会占据n*m + 1个位置 
//		for(int i = 1; i <= n*m; i++){
//			element[i].row = -1;
//		}
		rows = n; cols = m;
		int cnt = 1;//记录数组的索引
		int cinvalue; 
		for(int i = 1; i<= n; i++){
			for(int j = 1; j <=m; j++){
				cinvalue = Read();
				if(cinvalue != 0){
					realnum++;
					element[cnt].row = i;
					element[cnt].col = j;
					element[cnt].value = cinvalue;
					cnt++;
				}
			}
		}
	}
	
	~spareMatrix(){delete []element;}
	
	int getrows(){return rows;}
	matrixTerm* getElement(){return (element + 1);}
	int getcols(){return cols;}
	
	class iterator{
	public:
		matrixTerm* ptr;
		iterator(){};//空构造函数 
		void get(matrixTerm* temp){
			ptr = temp;
		}
		iterator & operator ++(int){
			iterator* temp  =this;
			ptr++;
			return *temp;
		}
		iterator& operator++(){
			ptr++;
			return *this;
		}
		matrixTerm &operator*()const{
			return *ptr;
		}
	};
//void MatrixMultiply(){
//	spareMatrix *sp1 = new spareMatrix;
//	sp1->createMatrix();//将本操作所在稀疏矩阵作为左侧矩阵，sp1矩阵作为右侧矩阵,result矩阵作为结果矩阵 
//	if(cols != sp1->getrows()){
//		delete element;
//		*this = *sp1;
//		printf("-1\n");
//		return;
//	}
//	spareMatrix* result = new spareMatrix;
//	
//	
//}	
void MatrixMultiply(spareMatrix *& A, spareMatrix * &B){//A是已知矩阵，B是新矩阵, 最后的结果记录到调用函数的矩阵C中，并且不是A，B中任意一个矩阵调用的该函数 
		if(A->getcols() != B->getrows()){//如果A的列不等于B的行，则不能做矩阵加法; 
		   	delete A;						//释放A的空间 
			*this = *B;						//直接让this等于指针B 
			printf("-1\n");			//不合法输出-1 
			return;
		}
		rows = A->getrows();
		cols = B->getcols();
		element = new matrixTerm[A->getrows() * B->getcols() + 1];
//		for(int i = 1; i <= A->getrows() * B->getcols(); i++){
//			element[i].row = -1;
//		}
		spareMatrix::iterator ptrA; spareMatrix::iterator ptrB;
		int cnt = 1;//记录 
		for(int i = 1; i <= A->getrows(); i++){
			for(int j = 1; j <= B->getcols(); j++){
				int tempans = 0;//记录乘积之和
				ptrA.get(A->getElement()); ptrB.get(B->getElement());
				
				for(int k = 1; k <= A->getcols(); k++){//k既是A的列，也是B的行 
					//首先把A，B调整到相应位置,判断row!= -1 是判断是否到指针末尾
					int tempA = -1; int tempB = -1;//-1判断数组中是否存在该元素，如果没有，则将-1变为0； 
				//	std::cout<<"";
					while((*(ptrA)).row!= -1 && (*(ptrA)).row < i) ptrA++;
					while((*(ptrA)).row!= -1 && (*(ptrA)).row ==i && (*(ptrA)).col < k) ptrA++;//此时ptrA可能在目标位置，也可能超过目标位置  
					while((*(ptrB)).row!= -1 && (*(ptrB)).row < k) ptrB++;
					while((*(ptrB)).row!= -1 && (*(ptrB)).row == k &&(*(ptrB)).col < j) ptrB++;//此时ptrB可能在目标位置，也可能超过目标位置 
					if((*(ptrA)).row == i && (*(ptrA)).col == k && (*(ptrB)).row == k && (*(ptrB)).col == j){
						tempans += ((*(ptrA)).value) * ((*(ptrB)).value); 
					}
				}
				if(tempans != 0){
					//std::cout<<"tempans"<<tempans<<'\n';
					realnum++;
					element[cnt].row = i; 
					element[cnt].col = j;
					element[cnt].value = tempans;
					cnt++;
				}
			}
		}
		delete A; delete B;
	}

void MatrixPlus(spareMatrix*& A, spareMatrix*& B){
	if(A->getrows() != B->getrows() || A->getcols() != B->getcols()){//如果A,B的行列不相等 
		  	delete A;						//释放A的空间 
			*this = *B;						//直接让this等于指针B 
			printf("-1\n");			//不合法输出-1 
			return ;
		}
	rows = A->getrows();//A，B行列相等 
	cols = A->getcols();
	element = new matrixTerm[rows * cols + 1];
//	for(int i = 1; i <= rows*cols; i++){
//		element[i].row = -1;
//	}
	spareMatrix::iterator ptrA; spareMatrix::iterator ptrB;
	ptrA.get(A->getElement()); ptrB.get(B->getElement());
	int cnt = 1;
	for(int i = 1; i <= rows; i++){
		for(int j = 1; j <= cols; j++){
			while((*(ptrA)).row != -1 && (*(ptrA)).row < i) ptrA++;
			while((*(ptrA)).row != -1 && (*(ptrA)).row == i &&((*(ptrA)).col) < j)ptrA++ ;
			while((*(ptrB)).row != -1 && (*(ptrB)).row < i) ptrB++;
			while((*(ptrB)).row != -1 && (*(ptrB)).row == i &&((*(ptrB)).col) < j)ptrB++ ;//此时ptrA，ptrB位于目标位置或更前方位置
			if(((*(ptrA)).row == i && (*(ptrA)).col == j) ||((*(ptrB)).row == i && (*(ptrB)).col == j)){
				int temp = 0, tempA = 0, tempB = 0;
				if((*(ptrA)).row == i && (*(ptrA)).col == j) tempA = (*(ptrA)).value;
				if((*(ptrB)).row == i && (*(ptrB)).col == j) tempB = (*(ptrB)).value;
				temp = tempA+ tempB;
				if(temp != 0){
					realnum++;
					element[cnt].row = i;
					element[cnt].col = j;
					element[cnt].value = temp;
					cnt++;
				}
			} 
		}
	}
	delete A;delete B;
}

void MatrixInverse(){
	int *colSize = new int[cols + 1];
	int *rowNext = new int[cols + 1];
	for(int i = 1; i <= cols ; i++){
		colSize[i] = 0;
	}
//	for(int i = 1; i <= rows*cols + 1; i++){
//		if(element[i].row == -1) break;
//		realnum++;
//	}
	for(int i = 1; i <= realnum; i++){
		colSize[element[i].col] ++;
	}
	rowNext[1] = 1;
	for(int i = 2; i <= cols; i++){
		rowNext[i] = rowNext[i - 1] + colSize[i - 1]; 
	}
	matrixTerm* temp = new matrixTerm[cols * rows + 1];
	for(int i = 1; i<= cols*rows + 1; i++){
		temp[i].row = -1;
	}
	for(int i = 1; i <= realnum; i++){
		temp[rowNext[element[i].col]].col = element[i].row;
		temp[rowNext[element[i].col]].row = element[i].col;
		temp[rowNext[element[i].col]].value = element[i].value;
		rowNext[element[i].col] ++;
	}
	int cntrows = rows;
	rows = cols;
	cols = cntrows;
	delete []element;
	delete[]colSize;
	delete[]rowNext;
	element = temp;
	return;
}

	void OutMatrix(){
		printf("%d",rows);
		printf(" ");
		printf("%d\n",cols);
		int cnt = 1;
		for(int i = 1; i<= rows; i++){
			for(int j = 1; j <= cols; j++){
				if(element[cnt].row == i && element[cnt].col == j){
					printf("%d ",element[cnt].value);
					cnt++;
				}
				else{
					printf("0 ");
				}
			}
			printf("\n");
		}
	}
	void createMatrix(){
		rows = Read(); cols = Read();
		int t;realnum = t;
		t = Read();
		element = new matrixTerm[t + 1];
		int x, y, v;
		for(int i = 1; i<=t; i++){
			x = Read(); y = Read(); v = Read();
			element[i].row = x;
			element[i].col = y;
			element[i].value = v;
		}
	}
	int realnum = 0;;//记录稀疏矩阵真实数据个数 
private:
	int rows;
	int cols;
	matrixTerm* element = nullptr;
};
int main(){
	int w;
	w = Read();
	int sym;
	spareMatrix *mt = new spareMatrix;
	while(w){
		w--;
		sym  = Read();
		if(sym == 1){
			mt->resetMatrix();
		}
		else if(sym == 2){
			spareMatrix *newmt = new spareMatrix;
			newmt->createMatrix();
			spareMatrix* temp = new spareMatrix;
			temp->MatrixMultiply(mt,newmt);
			mt = temp;
		}
		else if(sym == 3){
			spareMatrix * newmt = new spareMatrix;
			newmt->createMatrix();
			spareMatrix* temp = new spareMatrix;
			temp->MatrixPlus(mt, newmt);
			mt = temp;
		}
		else if(sym == 4){
			mt->OutMatrix();
		}
		else if(sym ==5){
			mt->MatrixInverse();
		}
	}
	delete mt;
//	std::cout<<"ok";
	return 0;
}
