#include<iostream>
using namespace std;
struct matrixTerm
{
    int row,col,value;//三元组结构体定义
    matrixTerm& operator=(matrixTerm& x)
    {//运算符重载 ，赋值*this = x
        row=x.row;
        col=x.col;
        value=x.value;
        return *this;
    }
};
const int maxsize = 199999;
class sparseMatrix
{
    private:
    int rows;//行
    int cols;//列
    int terms;//非0元素数量
    matrixTerm *element;
    int maxSize;
    public:
    sparseMatrix(int n,int m)//构造函数
    {
        element = new matrixTerm[maxsize];
        rows=n;
        cols=m;
        terms=0;
    }
    ~sparseMatrix(){delete [] element;}//析构函数
    sparseMatrix& operator=(sparseMatrix& x) 
    {//运算符重载 ，赋值*this = x
        rows=x.rows;
        cols=x.cols;
        terms=x.terms;
        maxSize=x.maxSize;
        for(int i =0;i<terms;i++)
        element[i]=x.element[i];
        return *this;
    }
    void insert(int x,int y,int v)
    {
        matrixTerm temp;
        temp.row=x;
        temp.col=y;
        temp.value=v;
        element[terms]=temp;
        terms++;//
    }
    void reset1(int n,int m)
    {
        rows=n;
        cols=m;
        terms=0;
        for(int i=1;i<=rows;i++)
        for(int j=1;j<=cols;j++)
        {
            int val;
            cin>>val;
            if(val!=0)
            {
                element[terms].row=i;
                element[terms].col=j;
                element[terms].value=val;
                terms++;
            }
        }
    }
    int multiply2(sparseMatrix& Q)
    {
        //检查矩阵是否可乘
        if(cols!=Q.rows)
        {*this=Q;
        return -1;}
        else
        {
            sparseMatrix R(rows,Q.cols);//结果矩阵
            //计算Q矩阵每一行的非0元素个数rowSize
            int rowSize[10000];
            for(int i=1;i<=Q.rows;i++)
            rowSize[i]=0;
            for(int i=0;i<Q.terms;i++)
            {
            if (Q.element[i].value != 0) 
            rowSize[Q.element[i].row]++;
            }
            //计算Q矩阵每一行的首个非0元素偏移rowNext
            int rowNext[10000];
            rowNext[1]=0;
            for(int i=2;i<=Q.rows;i++)
            rowNext[i]=rowNext[i-1]+rowSize[i-1];
            //结果数组保存 矩阵i行与各列乘法结果
            int answer[10000];

            int p=0;//遍历矩阵P
            for(int i=0;i<rows && p<terms;i++)//遍历P每行，且有剩余非0元素
            {
                //重置结果数组
                for(int j=1;j<=Q.cols;j++)
                answer[j]=0;
                //计算i行每列的乘积
                while(p<terms && element[p].row==i)
                {
                    int t = element[p].col;//当前元素P中的列，即Q中对应的行
                    if(rowSize[t]!=0)//若Q中该行元素个数为0，则不需要计算i行每列的乘积
                    {
                        for(int q = rowNext[t];q<rowNext[t]+rowSize[t];q++)//<=
                        answer[Q.element[q].col]+= element[p].value * Q.element[q].value;
                    }
                    p++;
                }
                //遍历answer，将其中非0元素插入结果矩阵
                for(int k=1;k<=Q.cols;k++)
                {
                    if(answer[k]!=0)
                    {
                        R.element[R.terms].row=i;
                        R.element[R.terms].col=k;
                        R.element[R.terms].value=answer[k];
                        R.terms++;
                    }
                }
            }
            *this=R;
            return 0;
        }
    }

    int add3(sparseMatrix& Q)
    {
        //检验相容性
        if(rows!=Q.rows || cols!=Q.cols)
        {*this=Q;
        return -1;}
        else
        {
            //设置结果矩阵特征
            sparseMatrix c(rows,cols);
            //'迭代器'
            int it=0;
            int ib=0;
            //遍历*this 和 Q，把相关项相加
            while(it != terms && ib != Q.terms)
            {
                //计算索引：行主索引加上每一项列数
                int tIndex=(element[it]).row*cols + (element[it]).col;
                int bIndex=(Q.element[ib]).row*Q.cols + (Q.element[ib]).col;
                //根据两索引大小，决定将谁插入结果矩阵c
                //Q在后
                if(tIndex<bIndex)
                {
                    c.element[c.terms].col=element[it].col;
                    c.element[c.terms].row=element[it].row;
                    c.element[c.terms].value=element[it].value;
                    it++;
                    c.terms++;//c.
                }
                //两项在同一个位置
                 else if(tIndex==bIndex)
                {
                //仅当相加不为0时插入c
                    if((element[it].value+Q.element[ib].value)!=0)
                    {
                    c.element[c.terms].col=element[it].col;
                    c.element[c.terms].row=element[it].row;
                    c.element[c.terms].value=element[it].value+Q.element[ib].value;
                    c.terms++;
                    }
                    it++,ib++;
                }
                //t在后
                else
                {
                    c.element[c.terms].col=element[ib].col;
                    c.element[c.terms].row=element[ib].row;
                    c.element[c.terms].value=element[ib].value;
                    ib++;
                    c.terms++;
                }
            }
                //复制剩余项
                for(;it!=terms;it++)
                {
                    c.element[c.terms].col=element[it].col;
                    c.element[c.terms].row=element[it].row;
                    c.element[c.terms].value=element[it].value;
                    c.terms++;
                }
                for(;ib!=Q.terms;ib++)
                {
                    c.element[c.terms].col=element[ib].col;
                    c.element[c.terms].row=element[ib].row;
                    c.element[c.terms].value=element[ib].value;
                    c.terms++;
                }
            
            *this=c;
            return 0;
        }
    }

    void output4()
    {
        int i,j;
        cout<<rows<<" "<<cols<<endl;
        int p =0;//遍历非0元素
        for(i = 1;i<=rows;i++) //遍历行
            {
                for(j = 1;j<=cols;j++)//遍历列
                {
                    if(p<terms && element[p].row==i && element[p].col==j)
                    {cout<<element[p].value<<" ";
                    p++;}
                    else 
                    cout<<0<<" ";
                }
                cout<<endl;
            }

    }

    void transpose5()
    {
        sparseMatrix b(cols,rows);
        b.terms=terms;
        b.cols=rows;
        b.rows=cols;
        int* colSize = new int [cols+1];//矩阵i列非0元素个数
        int* rowNext = new int [cols+1];//矩阵第i列第一个非0元素索引
        //计算colSize
        for(int i=1;i<=cols;i++)
        colSize[i]=0;
        for(int i=0;i<terms;i++)
        colSize[element[i].col]++;
        //计算rowNext
        rowNext[1]=0;
        for(int i=2;i<=cols;i++)
        rowNext[i]=rowNext[i-1]+colSize[i-1];
        //遍历非0元素，实现转置
        for(int i=0;i<terms;i++)
        {
            int j =rowNext[element[i].col];
            b.element[j].row=element[i].col;
            b.element[j].col=element[i].row;
            b.element[j].value=element[i].value;
            rowNext[element[i].col] = rowNext[element[i].col] + 1;
        }
        *this=b;
    }
};

int main()
{
    sparseMatrix P(0,0);
    int w,n,m,t,x;
    int feedback=0;//接收函数传回数值
    cin>>w;
    for(int i = 0;i<w;i++)
    {
        cin>>x;
        if(x==1)
        {
        cin>>n>>m;
        P.reset1(n,m);
        }
        else if(x==2)
        {
        cin>>n>>m;
        cin>>t;
        sparseMatrix Q1(n,m);
        for(int j =0;j<t;j++)
        {
            int x,y,v;
            cin>>x>>y>>v;
            Q1.insert(x,y,v);
        }
            feedback=P.multiply2(Q1);
            if(feedback==-1)
            cout<<feedback<<endl; //写在循环外面
        }
        else if(x==3)
        {
        cin>>n>>m;
        cin>>t;
        sparseMatrix Q2(n,m);
        for(int j =0;j<t;j++)
        {
            int x,y,v;
            cin>>x>>y>>v;
            Q2.insert(x,y,v);
        }
            feedback=P.add3(Q2);
            if(feedback==-1)
            cout<<feedback<<endl;
        }
        else if(x==4)
        {
        P.output4();
        }
        else
        {
        P.transpose5();
        }
    }
    return 0;
}


