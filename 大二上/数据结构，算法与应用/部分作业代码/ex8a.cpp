#include<iostream>

class hash{
public:
	hash(int D){
		element = new int[D];
		sym = new bool[D];
		for(int i = 0; i<D; i++){
			sym[i] = 0;
		}
		arrayLength = D;
	}
	~hash(){
		delete []element;
		delete []sym;
	}
	void insert(int x){
		int mapnum = x % arrayLength;
		int countnum = 0;//判断是否循环过一遍 
		while(countnum < arrayLength && sym[mapnum] == 1){
			if(element[mapnum] == x){
				std::cout<<"Existed"<<'\n';
				return ;
			}
			countnum++;
			mapnum++; mapnum = mapnum % arrayLength;
		}//直到循环过一遍或者找到了一个空位停止 
		if(sym[mapnum] == 0){
			element[mapnum] = x;
			sym[mapnum] = 1;
			std::cout<<mapnum<<'\n';
		}
		return;
	}

	void search(int x){
		int mapnum = x % arrayLength;
		int countnum = 0;//用来判断循环是否循环过一遍 
		while(countnum < arrayLength){
			if(sym[mapnum] == 0){
				std::cout<<"-1"<<'\n';
				return;
			}
			if(element[mapnum] == x){
				std::cout<<mapnum<<'\n';
				return;
			}
			countnum++; mapnum++;
			mapnum = mapnum % arrayLength; 
		}
		std::cout<<"-1"<<'\n';//遍历了整个数组都没有找到
		return;  
	}
	//遍历到位置的元素索引和初始桶做减法，如果>0，要求空位在初始桶后，<0空位在初始桶前 
	void deleteelement(int x){
		int mapnum = x % arrayLength;
		int countnum = 0;//用来判断循环是否循环过一遍
		while(countnum < arrayLength && sym[mapnum] == 1){
			if(element[mapnum] == x){
				break;
			}
			countnum++; mapnum++;
			mapnum = mapnum % arrayLength;
		}
		if(countnum == arrayLength || sym[mapnum]==0){
			std::cout<<"Not Found"<<'\n';
			return;
		}
		//剩下的情况是找到且 mapnum就是x对应的索引
		//进行移动操作
		int count = 0;//记录移动个数 
		sym[mapnum] = 0;//删除该位置元素，即将该位置的sym置为0
		int last = mapnum;//用来记录空缺桶位置
		mapnum++; mapnum = mapnum % arrayLength;
		countnum =  0;//统计是否完成表遍历
		while(countnum < arrayLength && sym[mapnum] == 1){//没有遇到空桶且没有完成一次遍历
			int start = element[mapnum] % arrayLength; 
			//如果1 初始桶在空桶前方，并且空桶在遍历元素前方 
			//如果3 空桶在所遍历元素前方并且遍历元素在初始桶前方
			//如果2 遍历元素在初始桶前方并且初始桶在空桶前方
			//mapnum遍历位置， startbucket初始桶， last 空桶 
			if((start <= last && last < mapnum) || (last < mapnum && mapnum < start) || (mapnum < start && start <= last)){//如果该元素的初始桶位置在空缺位置左边 
				element[last] = element[mapnum];
				sym[last] = 1;
				sym[mapnum] = 0;
				last = mapnum;
				mapnum++; mapnum = mapnum % arrayLength;
				count++;
			}
			else{
				mapnum++; mapnum = mapnum % arrayLength;
			}
		} 
		std::cout<<count<<'\n';
		return;
	}
	void Outhash(){
		for(int i = 0; i < arrayLength; i++){
			if(sym[i]== 1)
			std::cout<<element[i]<<' ';
			else 
			std::cout<<'-'<<' ';
		}
		std::cout<<'\n';
	}

private:
	int* element;//用来记录表 
	bool * sym;//用来记录桶是否空,0 为空， 1为占用
	int arrayLength;//记录哈希表的表长度,用来取模值 
};
int main(){
	int D, m;
	std::cin>>D>>m;
	hash h(D);
	int opt; int x;
	for(int i = 1; i<=m; i++){
		std::cin>>opt>>x;
		if(opt == 0) h.insert(x);
		else if(opt == 1) h.search(x);
		else if(opt == 2) {
		h.deleteelement(x);
		}
	}
	return 0;
}
