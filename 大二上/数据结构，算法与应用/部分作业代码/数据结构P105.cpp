11
void arrayList<T>::push_back(int theElement){
	if(arrayLength == listSize){
		changeLength1D(element, arraylength, arraylength + 1);
	}
	*(element + listSize) = theElement;//O1的时间复杂度 
}
12
void arrayList<T>::pop_back(){
	*(element + listSize -1)  = 0;
	listSize -= 1;//01的时间复杂度 
}
14
void arrayList<T>::reserve(T *&a, int theCapacity){
	if(theCapacity <= arrayLength) return ;
	T* temp = new T[theCapacity];
	copy(a, a+listSize,temp);
	delete []a;
	a = temp; 
}

16
void arrayList<T>::clear(){
	delete[]element;
	arrayLength = 0;
	listSize = 0;
}



