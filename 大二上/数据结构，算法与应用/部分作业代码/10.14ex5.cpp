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
};
void swap(matrixTerm &a, matrixTerm &b){
	matrixTerm temp = a;
	a = b;
	b = temp;
}
class spareMatrix{
public:
	void resetMatrix(){//重置数组 
		int n, m;
		n = Read(); m = Read();
		delete []element;
		element = new matrixTerm[n*m + 1];//最多会占据n*m + 1个位置 
		rows = n; cols = m;
		int cnt = 1;//记录数组的索引
		int cinvalue; 
		for(int i = 1; i<= n; i++){
			for(int j = 1; j <=m; j++){
				cinvalue = Read();
				if(cinvalue != 0){
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
		matrixTerm* operator->()const{
			return ptr;
		}
	};
	
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
		matrixTerm* ptrA = A->element + 1; matrixTerm* ptrB = B->element + 1;
		int cnt = 1;//记录 
		for(int i = 1; i <= A->rows; i++){
			for(int j = 1; j <= B->cols; j++){
				int tempans = 0;//记录乘积之和
				ptrA = A->element + 1; ptrB = B->element + 1;
				for(int k = 1; k <= A->getcols(); k++){//k既是A的列，也是B的行 
					//首先把A，B调整到相应位置,判断row!= -1 是判断是否到指针末尾
				//	int tempA = -1; int tempB = -1;//-1判断数组中是否存在该元素，如果没有，则将-1变为0； 
				//	std::cout<<"";
					while(ptrA->row!= -1 && ptrA->row < i) ptrA++;
					while(ptrA->row!= -1 && ptrA->row ==i && ptrA->col < k) ptrA++;//此时ptrA可能在目标位置，也可能超过目标位置  
					while(ptrB->row!= -1 && ptrB->row < k) ptrB++;
					while(ptrB->row!= -1 && ptrB->row == k &&ptrB->col < j) ptrB++;//此时ptrB可能在目标位置，也可能超过目标位置 
					
					if(ptrA->row == i && ptrA->col == k && ptrB->row == k && ptrB->col == j){
						tempans += ptrA->value * ptrB->value; 
					}
				}
				if(tempans != 0){
					//std::cout<<"tempans"<<tempans<<'\n';
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
	if(A->rows != B->rows || A->cols != B->cols){//如果A,B的行列不相等 
		  	delete A;						//释放A的空间 
			*this = *B;						//直接让this等于指针B 
			printf("-1\n");			//不合法输出-1 
			return ;
		}
	rows = A->rows; cols = B->cols;
	element = new matrixTerm[A->rows * B-> cols + 1];
	matrixTerm* ptrA = A->element + 1; matrixTerm* ptrB = B->element + 1; 
	int cnt = 1;
	for(int i = 1; i <= rows; i++){
		for(int j = 1; j <= cols; j++){
			while(ptrA->row != -1 && ptrA->row < i) ptrA++;
			while(ptrA->row != -1 && ptrA->row == i && ptrA->col < j)ptrA++ ;
			while(ptrB->row != -1 && ptrB->row < i) ptrB++;
			while(ptrB->row != -1 && ptrB->row == i && ptrB-> col < j)ptrB++ ;//此时ptrA，ptrB位于目标位置或更前方位置
			if((ptrA->row == i && ptrA->col == j) ||(ptrB->row == i && ptrB->col == j)){
				int temp = 0, tempA = 0, tempB = 0;
				if(ptrA->row == i && ptrA-> col == j) tempA = ptrA->value;
				if(ptrB->row == i && ptrB->col == j) tempB = ptrB->value;
				temp = tempA+ tempB;
				if(temp != 0){
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

void MatrixInverse(){//一个数组中有元素的矩阵来调用
	int cntrows = rows;
	rows = cols;
	cols = cntrows; 
	for(int i = 1; i <= rows*cols + 1; i++){
		if(element[i].row == -1) break;
		int temp = element[i].row;
		element[i].row = element[i].col;
		element[i].col = temp;
	}				//元素的行列号互换
	int sym = 1;
	for(int i = 1; i <= rows * cols + 1 && sym == 1; i++){
		sym = 0;
		for(int j = 1; j <= rows * cols; j++){
			if(element[j + 1].row == -1) break;
			else if(element[j].row > element[j + 1].row) {swap(element[j],element[j + 1]);sym = 1;}
			else if(element[j].row == element[j + 1].row && element[j].col > element[j + 1].col) {swap(element[j],element[j + 1]); sym = 1;}
		}
	}
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
	void createMatrix(int newrow, int newcol){
		rows = newrow; cols = newcol;
		element = new matrixTerm[newrow*newcol + 1];
		int t;
		t = Read();
		int x, y, v;
		for(int i = 1; i<=t; i++){
			x = Read(); y = Read(); v = Read();
			element[i].row = x;
			element[i].col = y;
			element[i].value = v;
		}
	}
	
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
			int nmtrow, nmtcol;
			nmtrow = Read(); nmtcol = Read();
			spareMatrix *newmt = new spareMatrix;
			newmt->createMatrix(nmtrow, nmtcol);
			spareMatrix* temp = new spareMatrix;
			temp->MatrixMultiply(mt,newmt);
			mt = temp;
		}
		else if(sym == 3){
			int nmtrow, nmtcol;
			nmtrow = Read(); nmtcol = Read();
			spareMatrix * newmt = new spareMatrix;
			newmt->createMatrix(nmtrow, nmtcol);
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
	return 0;
}
