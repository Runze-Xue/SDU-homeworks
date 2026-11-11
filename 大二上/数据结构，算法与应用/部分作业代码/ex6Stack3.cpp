#include<iostream>

struct Node{
double data;//记录数据 
char ope;//记录运算符,如果值为0，则说明该点记录的是数据，否则是符号 
};

template<typename T>
class Stack{
public:
	Stack(){
		element = new T[2005];//题目声明不超过2000 
		listSize = 0 ;
		head = 0;
		tail = 0;
	}
	~Stack(){delete []element;}
	bool empty()const{return listSize == 0;}
	void push(T& a){
		tail++;
		element[tail] = a; 
		listSize++;
	}
	void pop(){
		if(!empty()){
			tail--;
			listSize--;
		}
	}
	T& top(){
		return element[tail];
	}
	void clear(){//清空栈 
		if(!this->empty()){
			this->pop();
		}
	}
	void OutStack(){
		while(!this->empty()){
			std::cout<<this->top();
			this->pop();	
		}
	}
	double calc(){//计算一个括号内的值 
		double temp = 0;//计算括号内运算结果,单项式结果
		double tempans = 0;
		double multidiv = 1;//特殊记录乘除法运算结果 
		int sym = 0;//1 代表上一次操作为乘除法运算 
		temp = this->top().data;
		this->pop();//最右侧数记录并弹出 
		while( this->top().ope !='(' && !this->empty()){//当没有弹到'('则重复弹栈操作
			 if(this->top().ope=='+'){
			 	if(sym == 1){
			 		sym = 0;
			 		multidiv *= temp;
			 		tempans += multidiv;
			 		multidiv = 1;
			 		this->pop();//弹出加号
					temp = this->top().data;
					this->pop();//弹出该数 
				 }
				 else{
					tempans += temp;//
					this->pop();//弹出加号
					temp = this->top().data;
					this->pop();//弹出该数 
				}
			}
			else if(this->top().ope=='-'){
				if(sym == 1){
					sym = 0;
					multidiv *= temp;
					tempans -= multidiv;
					multidiv = 1;
					this->pop();
					temp = this->top().data;
					this->pop();//弹出该数 
				}
				else{
					tempans -= temp;
					this->pop();//弹出减号
					temp = this->top().data;
					this->pop();//弹出该数
			}
			}
			else if(this->top().ope=='*'){
				sym = 1;
				this->pop();//弹出乘号
				multidiv *= temp;
				temp = (this->top().data );
				this->pop();//弹出该数 
			}
			else if(this->top().ope=='/'){
				sym = 1;
				this->pop();//弹出除号
				multidiv = 1.0 * multidiv / temp;
				temp = this->top().data;
				this->pop();//弹出该数 
			}
		}
		//将括号内第一个单项式加入括号和中 
		if(sym == 1){
			tempans += multidiv *temp;
		}
		else{
			tempans += temp;
		}
		return tempans;
	}//calc操作不弹出括号 
	void Solution(){
		char c = getchar();
		while(c !='\n'){//当接收到的不是回车键时继续接受 
			if(c == ')'){//当c是一个右边括号时
				double tempans = this->calc();
				Node temp;
				temp.ope = 0;
				temp.data = tempans;
				this->pop();//弹出左括号
				this->push(temp); 
				c = getchar();
				continue;
			}
		   if(c >= '0' && c <= '9'){//说明压入的是一个数 
				Node temp;
				temp.ope = 0;
				temp.data = c -'0';
				this->push(temp);
			}
			else{
				Node temp;
				temp.ope = c;
				this->push(temp);
			}
			c = getchar();
		}
		double ans = this->calc();
		printf("%.2f\n",ans);
		return;
	}

private:
	T *element = nullptr;
	int listSize = 0;
	int head = 0;//头是栈的第一个元素索引，一般不进行操作 
	int tail = 0;//尾是数组尾,随着元素压入、弹出栈而不断改变 
}; 
int main(){
	Stack<Node> st;
	int n;
	std::cin>>n;
	char c = getchar();
	for(int i = 1; i<=n; i++){

		st.clear();
		st.Solution();
	}
	return 0;
	
} 
