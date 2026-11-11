#include<iostream>
using namespace std;
template<class T>
class vector{
	public:
		T& operator[](int a)const{
			return p[a];
		}
		vector<T>& operator = (const vector<T>& arr);
		vector();
		vector(const vector<T>& q);
		~vector(){
			delete []p;
			p=nullptr;
			s=0;
			capacity=0;
			
		}
		void pop_back();
		void push_back(T a);
		int capa()const{
			return capacity;
		}
		int size()const{
			return s;
		}
	private:
		int s;
		int capacity;
		T *p;
};
template<class T>
vector<T>& vector<T>::operator = (const vector<T>& arr) {
    delete[] p;
    s = arr.size();
    capacity = arr.capa();
    p = new T[capacity];
    for (int i = 0; i < s; ++i) {
        p[i] = arr.p[i];
    }
    return *this;
}
template<class T>
void vector<T>::pop_back(){
	if(s>0){
		s--;
	}
}
template<class T>
vector<T>::vector(const vector<T>& q){
	s=q.size();
	capacity=q.capa();
	p=nullptr;
	p=new T[q.capa()];
	for(int i=0;i<s;i++){
		p[i]=q.p[i];
	}
}
template<class T>
vector<T>::vector(){
	s=0;
	p=NULL;
	capacity=20;
}
template<class T>
void vector<T>::push_back(T a){
	if(p==NULL){
		s=0;
		capacity=20;
		p=new T[capacity];
	}
	if(s>=capacity){
		capacity=capacity*2;
		T *np=new T[capacity];
        for(int i = 0; i < s; i++){
            np[i] = p[i];
        }
        delete []p;
		p=np;
	}
	p[s]=a;
	s++;
}
struct element{
	int row;
	int col;
	int num;
	element(){row=0;col=0;num=0;}
	element(int a,int b,int c){
		row=a;
		col=b;
		num=c;
	}
};
class sparsematrix{
	public:
		sparsematrix &operator=( sparsematrix &q); 
		~sparsematrix(){matrix.~vector();}
		sparsematrix(){rows=0;cols=0;vector<struct element>matrix;}
		sparsematrix(sparsematrix &q);
		void push(int a,int b,int c);
		void set(int a,int b);
		void show(); 
		void clear(); 
		void transpose();
		sparsematrix& operator+( sparsematrix &q);
		void multiply(sparsematrix &q);
	private:
		int rows=0;
		int cols=0;
		vector<struct element>matrix;
};
void sparsematrix::multiply(sparsematrix &q){
	if(cols!=q.rows){
		cout<<-1<<endl;
		matrix=q.matrix;
		return ;
	}
	else{
		sparsematrix result;
		q.transpose();
		int *lrowsize=new int[rows+10];//左侧矩阵每行元素个数 
		int *lrownext=new int[rows+10]; //左侧矩阵每行元素开头
		int *rrowsize=new int[q.rows+10];//右侧矩阵每行元素个数
		int *rrownext=new int[q.rows+10];//右侧矩阵每行元素开头
		for(int i=0;i<rows;i++){
			lrowsize[i]=0;
			rrowsize[i]=0;
		}//初始化为0 
		for(int i=0;i<matrix.size();i++){
			lrowsize[matrix[i].row]++;
		}//hang
		for(int i=0;i<q.matrix.size();i++){
			rrowsize[q.matrix[i].row]++;
		}//youcehang
		lrownext[0]=0;
		rrownext[0]=0;
		for(int i=1;i<rows;i++){
			lrownext[i]=lrownext[i-1]+lrowsize[i-1];
		}
		for(int i=1;i<q.rows;i++){
			rrownext[i]=rrownext[i-1]+rrowsize[i-1];
		}
		for(int i=0;i<rows;i++){
			for(int j=0;j<rows;j++){
				int sum=0;
				int num1=0,num2=0;
				while(num1<lrowsize[i]&&num2<rrowsize[j]){
					//位置相等就加进去 
					int po1=0,po2=0;
					po1=matrix[lrownext[i]+num1].col;
					po2=q.matrix[rrownext[j]+num2].col;
					if(po1==po2){
						sum=sum+matrix[lrownext[i]+num1].num*q.matrix[rrownext[j]+num2].num;
						num1++;
						num2++;
					}
					else if(po1<po2)num1++;
					else if(po1>po2)num2++;
				}
				if(sum!=0)
				result.push(i,j,sum);
			}
		}
		matrix=result.matrix;		
	}
}
void sparsematrix::transpose(){
	sparsematrix b;
	b.cols=rows;
	b.rows=cols;
	int *colsize=new int[cols+10];
	int *rownext=new int[cols+10];
	for(int i=0;i<cols;i++){
		colsize[i]=0;
	}
	for(int i=0;i<matrix.size();i++){
		struct element c;
		b.matrix.push_back(c);
		colsize[matrix[i].col]++;
	}
	rownext[0]=0;
	for(int i=1;i<cols;i++){
		rownext[i]=rownext[i-1]+colsize[i-1];
	}
	for(int i=0;i<matrix.size();i++){
		b.matrix[rownext[matrix[i].col]].col=matrix[i].row;
		b.matrix[rownext[matrix[i].col]].row=matrix[i].col;
		b.matrix[rownext[matrix[i].col]].num=matrix[i].num;
		rownext[matrix[i].col]++;
	}
	matrix=b.matrix;
}
sparsematrix::sparsematrix(sparsematrix &q){
	rows=q.rows;
	cols=q.cols;
	matrix=q.matrix;
}
sparsematrix& sparsematrix::operator+( sparsematrix &q){
	if(q.matrix.size()==0){
		return *this;
	}
	sparsematrix result; 
	if(rows!=q.rows||cols!=q.cols){
		cout<<-1<<endl;
		return q;
	}
	else{
		int num1=0,num2=0;//开始插入
		while((num1<matrix.size())&&(num2<q.matrix.size())){
			if((matrix[num1].row*cols+matrix[num1].col)==(q.matrix[num2].row*q.cols+q.matrix[num2].col)){
				result.push(matrix[num1].row,matrix[num1].col,matrix[num1].num+q.matrix[num2].num);
				num1++;
				num2++;
			}
			else if((matrix[num1].row*cols+matrix[num1].col)<(q.matrix[num2].row*q.cols+q.matrix[num2].col)){
				result.push(matrix[num1].row,matrix[num1].col,matrix[num1].num);
				num1++;
			}
			else if((matrix[num1].row*cols+matrix[num1].col)>(q.matrix[num2].row*q.cols+q.matrix[num2].col)){
				result.push(q.matrix[num2].row,q.matrix[num2].col,q.matrix[num2].num);
				num2++;
			}
		}
		while(num1<matrix.size()){
			result.push(matrix[num1].row,matrix[num1].col,matrix[num1].num);
			num1++;
		}
		while(num2<q.matrix.size()){
			result.push(q.matrix[num2].row,q.matrix[num2].col,q.matrix[num2].num);
			num2++;
		}
	}
	matrix=result.matrix;
	return *this;
}
sparsematrix& sparsematrix::operator=(sparsematrix &n){
	matrix.~vector();
	rows=n.rows;
	cols=n.cols;
	matrix=n.matrix;
	return *this;
}
void sparsematrix::clear() {
    while (matrix.size() > 0) {
        matrix.pop_back();
    }
}
void sparsematrix::show(){
	cout<<rows<<" "<<cols<<endl;
	int m=0;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(m<matrix.size()){
				if(matrix[m].row==i&&matrix[m].col==j){
					cout<<matrix[m].num<<" ";
					m++;
				}
				else cout<<0<<" ";
			}
			else cout<<0<<" ";
			
		}
		cout<<endl;
	}
}
void sparsematrix::set(int a,int b){
	rows=a;
	cols=b;
}
void sparsematrix::push(int a,int b,int c){
	struct element q(a,b,c);
	matrix.push_back(q);
}
int main() {
	int n, ch=0;
	sparsematrix q;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin>>ch;
		if(ch==1){
			q.clear();
			int row,col;
			cin>>row>>col;
			q.set(row,col);
			for(int j=0;j<row;j++){
				for(int k=0;k<col;k++){
					int num;
					cin>>num;
					if(num!=0){
						q.push(j,k,num);
					}
				}
			}
		}
		if(ch==2){
			int row,col;
			cin>>row>>col;
			sparsematrix newp;
			newp.set(row,col);
			int t=0;
			cin>>t;
			for(int j=0;j<t;j++){
				int num1,num2,num3;
				cin>>num1>>num2>>num3;
				newp.push(num1-1,num2-1,num3);
			}
			q.multiply(newp);			
		}
		if(ch==3){
			int row,col;
			cin>>row>>col;
			sparsematrix newp;
			newp.set(row,col);
			int t=0;
			cin>>t;
			for(int j=0;j<t;j++){
				int num1,num2,num3;
				cin>>num1>>num2>>num3;
				newp.push(num1-1,num2-1,num3);
			}
			sparsematrix ne=q;
			q=ne+newp;
			ne.~sparsematrix();
			newp.~sparsematrix();
		}
		if(ch==4){
			q.show();
		}
		if(ch==5){
			q.transpose();
		}
	}
}

