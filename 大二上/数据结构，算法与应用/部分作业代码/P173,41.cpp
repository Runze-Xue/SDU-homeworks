//P173 41题 get(theRow, theColumn) 
template<typename T>
T &sparseMatrix::get(theRow, theColumn){
	for(arrayList<matrixTerm<T>>::iterator i = terms.begin(); i != terms.end(); i++){//使用迭代器遍历数组
		if((i->row > theRow){
			std::cout<<"Wrong theRow"<<'\n';
			throw(int); //抛出一个int类型的异常,终止程序 
		}
		else if(i->row == theRow){
			if((i->col > theColumn){
				std::cout<<"Wrong theColumn"<<'\n';
				throw(int);//抛出一个int类型的异常 
			}
			else if((i->col == theColumn) return i->value;
		}
		 
	}
	throw(int);//如果遍历数组都没有找到，则抛出一个int类型异常 
}

// P173 41题 set(theRow., theColumn, theValue) 
template<typename T>
void sparseMatrix::set(theRow, theColumn, theValue){
	int index = 0;//找到插入位置的索引 
	int sym = 0; //判断是否从循环语句中终止 
	arrayList<matrixTerm<T>>::iterator i = terms.begin();//使用迭代器 
	for(; && i != terms.end(); i++){// 使用迭代器遍历数组 
		if(i->row > theRow) sym = 2;//如果被比较的元素行数大于给出行数，说明set的元素原来是0，此时要在稀疏矩阵中插入 
		else if(i->row == theRow){	//被比较的元素行数等于给出行数，则比较列数 
			if(i->col == theColumn) sym = 1;// 行列数与给出相同，此时找到了set元素，要更改其value值 
			else if(i->col > theColumn) sym = 2; //被比较元素的列数大于给出列数，说明set元素原来是0，此时要在稀疏矩阵中插入 
		}
		if(sym >= 1) break;			//结束遍历 
		index++;					//增加索引 
	}
	if(sym == 1){//sym = 1，说明找到了相应元素，对其值进行更改 
		i->value = theValue;//更新要求位置上元素value值为theValue 
	}
	else if(sym == 2){
	martixTerm<T>element;//不需要用new，因为所插入的是一个已经申请好空间的数组 
	element.row = theRow;
	element.col = theColumn;
	element.value = theValue;
	terms.insert(index, element);//使用arrayList类中的insert函数，将新元素插入到数组中 
	}
}

	
 


