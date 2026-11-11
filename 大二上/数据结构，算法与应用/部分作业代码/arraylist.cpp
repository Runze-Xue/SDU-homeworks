#include<iostream>

template<typename T>
class linearList{
public:
	virtual ~linearList(){};
	virtual bool empty()const = 0;
	virtual int size()const  = 0;
	virtual T& get(int index)const = 0;
	virtual int indexOf(const T& e)const  = 0;
	virtual void insert(int index, T& e)= 0 ;
	virtual void output(ostream &out)const  =0;
};

template<typename T>
class arrayList: public linearList<T>{
public:
	arrayList(int initCapacity = 10);
	arrayList(const arrayList<T>&);
	~arrayList(){ delete [] element;}
	
	bool empty()const{return listSize ==0};
	int size()const {return listSize ;}
	T & get(int index)const;
	int indexOf(const T & e)const;
	void erase(int index);
	void insert(int index, const T& e);
	void output(ostream &)const;
	int capacity()const {return arrayLength;}
	protected:
	void checkIndex(int index)const;
	private:
	T*element;
	int arrayLength;
	int listSize; 	
};
template<class T>
arrayList<T>::arrayList(int initCapacity){
	if(initCapacity < 1){
		ostringstream s;
		s <<"initCapacity = "<<initCapacity
		<<",must be > 0";
		throw illegalParameterValue(s.str());
	}
	arrayLength = initCapacity;
	element = new T[arrayLength];
	listSize = 0;
}
template<class T>
arrayList<T>::arrayList(const arrayList<T>& list){
	arrayLength = list.arrayLength;
	listSize = list.listSize;
	element = new T[arrayLength];
	copy(list.element, list.element+listSize,element);
}

template<class T>
void arrayList<T>::checkIndex(int index)const{
	if(index<0 || index>=listSize){
		ostringstream s;
		s<<"index = "<<index<<",size = "listSize;
		throw illegalIndex(s.str());
	}
}
template<class T>
T& arrayList<T>::get(int index)const
{
	checkIndex(intdex);
	return element[index];
}
template<T>::indexOf(const T&e)const{
	int index = find(element,element+listSize,e) - element;
	if(index == listSize) return -1;
	else return index ;
}
template<T>::erase(int index){
	checkIndex(index);
	copy(element+index + 1, element+listSize, element + index);
	element[--listSize].T();
}
template<class T>
void arrayList<T>::insert(int index, const T&e){
	if(index < 0 || index > listSize){
	
		ostringstream s;
		s<<"index = "<<index<<", size ="<<listSize;
		throw illegalIndex(s.str());
}
	if(listSize == arrayLength){
		changeLength1D(element, arrayLength,2*arrayLength);
		arrayLength *= 2; 
	}
	copy_backward(element + index, element + listSize, element + listSize +1);
}













