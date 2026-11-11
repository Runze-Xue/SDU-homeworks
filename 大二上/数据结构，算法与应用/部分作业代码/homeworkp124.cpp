
P124 3. 
template<typename T>
void chain<T>::set(theIndex, theElement){
	checkIndex(theIndex);   //如果索引超出范围，抛出异常
	T &temp = get(theIndex);
	temp = theElement;
}
//方法复杂度与get()函数寻找索引为theIndex的元素有关 复杂度为O（theIndex）

P125 15
template<typename T>
void chain<T>::reverse(){
	chainNode<T> *leftptr = nullptr;
	while(firstNode->next != nullptr){
		chainNode<T> *rightptr = firstNode->next;
		firstNode->next = leftptr;
		leftptr = firstNode;
		firstNode = rightptr;
	}
	firstNode->next = leftptr;     //  
	return ;
}
//方法的复杂度为O（listSize） 

P125 17
template<typename T>
void meld(const extendedChain<T> &a, const extendedChain<T> &b){
	extendedChain<T>* c = new extendedChain<T>;
	int Index_a = 0, Index_b = 0;
	while(Index_a < a.size() || Index_b < b.size()){
		if(Index_a < a.size()){
			T tempElement = a.get(Index_a);
			c.push_back(tempElement);
			Index_a++;
		}
		if(Index_b < b.size()){
			T tempElement = b.get(Index_b);
			c.push_back(tempElement);
			Index_b++;
		}
	}
	return ;
}
//

