//实验八 散列函数
#include<iostream>
 using namespace std;
 
template<class K,class E>
class hashTable{
	public:
		hashTable(int theDivisor=11)
		{
			divisor=theDivisor;//除数 
			dSize=0;
			//分配和初始化散列表数组
			table=new pair<const K,E>*[divisor]; 
			for(int i=0;i<divisor;i++)
			//将所有桶置空
			table[i]=NULL; 
		}
		
		void erase(const K &theKey);
		
		void find(const K&)const;
		//返回关键字theKey匹配的数对的指针，若不存在匹配的数对，则返回NULL 
		
		void insert(const pair<const K,E>&);
	protected:
		int search(const K&)const;
		pair<const K,E>**table;//散列表 
		int divisor;//散列表的除数 
	//	hash<K>hash;//把类型k映射到一个非负整数
		int dSize;//字典中数对的个数 
}; 

template<class K,class E>
int hashTable<K,E>::search(const K& theKey)const{
//搜索一个公开地址散列表，查找关键字为theKey的数对
//如果匹配的数对存在，则返回它的位置
//否则返回关键字为theKey的数对可以插入的位置
   int i=(int) theKey%divisor;//起始桶；
   int j=i;//从起始桶开始；
    do{
	  if(table[j]==NULL||table[j]->first==theKey)
	   return j;
	   j=(j+1)%divisor;//下一个桶 
     }while(j!=i);//又返回起始桶
     return j;//表已经满 
  } 


template<class K,class E>
void hashTable<K,E>::find(const K& theKey)const{
//返回关键字theKey匹配的数对的指针
//若不存在匹配的数对，则返回NULL
//搜索散列表
  int b=search(theKey);
  //判断table[b]是否是匹配数对
  if(table[b]==NULL||table[b]->first!=theKey) 
    cout<<"-1"<<endl;//未找到匹配数对
    
  else cout<<b<<endl;//找到匹配数对 

} 

template<class K,class E>
void hashTable<K,E>::insert(const pair<const K,E>&thePair)
{
	//在字典中插入一个数对thePair,若存在关键字相同的数对，
	//则覆盖，若表已满 ，则抛出异常
	int b=search(thePair.first);
	//检查匹配的数对是否存在
	//没有匹配的数对，且表不满，则插入数对
	if(table[b]==NULL)
	{
		table[b]=new pair<const K,E>(thePair);
		dSize++;
		cout<<b<<endl;
	 } 
	//有重复的关键字，则覆盖数对中的值
	else if(table[b]->first==thePair.first)
     cout<<"Existed"<<endl;	
	
 } 

template<class K,class E>
void hashTable<K,E>::erase(const K& theKey)
{
	int b=search(theKey);
	if(table[b]==NULL||table[b]->first!=theKey)
	cout<<"Not Found"<<endl;
	else 
	{
		table[b]=NULL;//删除；
		int x=b,y=b;//设置删除点，便于遍历
		b=(b+1)%divisor;//从删除的后一位开始遍历；
		int move=0;//记录移动个数 
		while(table[b]!=NULL&&b!=x)//没有遇到空桶且没有遍历一遍 
		{//如果遇到空桶说明后面肯定没有除余相同的数字; 
			//b索引中数字应该在的索引位置; 
			//b<m说明需要将m移动 
			//         {         (x)y   b     m1   }
			//         {  b    m     (x)  y        }
			//         {    m    y         b       }
			// y b m 1 |y m b  |b m y 1 | b y m | m y b 1| m b y
			int m=(int)table[b]->first%divisor;//原来的位置
			//移动后要靠近 
			//如果1 空桶在所遍历元素前方并且遍历元素在初始桶前方
			//如果2 遍历元素在初始桶前方并且初始桶在空桶前方
			//如果3 初始桶在空桶前方，并且空桶在遍历元素前方 
			if((y<b&&b<m)||(b<m&&m<=y)||(m<=y&&y<b))
			{
				table[y] =table[b];//将此时的table[b]放入table[y]中 [b]中空出 
				y=b;    //table[y]空出 
				move++;//移动一次 
			}
			
			b=(b+1)%divisor;//指针移动
			 
		}
		
		table[y]=NULL;//最后进行删除，使其等于空 
		cout<<move<<endl; 
	}
}



int main()
{
	
	int D,m;
	cin>>D>>m;
	hashTable<int,int> hash(D);
	while(m--)//操作次数 
	{
		int opt,x;
		cin>>opt>>x;
		
		switch(opt)
		{
		  case 0:
		  	{
		  	pair<int,int>p(x,0);
		  	hash.insert(p);	
			}
			break; 
		case 1:
			hash.find(x);
			break;	
	    case 2:
	    	hash.erase(x);
	    	break;
		} 
		
	}
	return 0;
 } 

























 
