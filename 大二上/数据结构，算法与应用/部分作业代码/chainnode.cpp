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
struct chainNode{
	T element;
	chainNode<T> *next;
	chainNode(){};
	chainNode(const T&e):element(e){};
	chainNode(const T&e, chainNode<T>*n):element(e),next(n){};
	~chainNode(){};
};
template<class T>
class chain:public linearList<T>{
public:
	chain(int capacity = 10);
	chain(const chain<T>&);
	~chain();
	
	bool empty()const {return listSize==0}
	int size()const{return listSize;}
	T&get(int index) const;
	void erase(int index);
	void insert(int index, const T &e);
	void output(ostream&out)const;
private:
	chainNode<T>*firstNode;
	int listSize;
};
template<typename T>
chain<T>::chain(int capacity){
	if(capacity < 1){
		ostringstream s;
		s<<"initCapacity = "<<capacity<<", must be >0";
		throw illegalParameterValue(s.str()); 
	}
	firstNode = nullptr;
	listSize = 0;
}
template<typename T>
chain<T>::chain(const chain<T>&list){
	listSize = list.listSize;
	if(listSize == 0){
		firstNode = nullptr;
		return;
	}
	chainNode<T>*sourceNode = list.firstNode;
	firstNode = new chainNode<T>(sourceNode->element);
	sourceNode = sourceNode->next;
	chainNode<T>*targetNode = firstNode;
	while(sourceNode!=nullptr){
		targetNode->next = new charinNode<T>(sourceNode->element);
		targetNode = targetNode->next;
		sourceNode = sourceNode->next;
	}
	targetNode->next = nullptr;
}
template<typename T>
chain<T>::~chain(){
	while(firstNode){
		chainNode<T>*next = firstNode->next;
		delete firstNode;
		firstNode = next;
	}
}
template<typename T>
int chain<T>::indexOf(const T&e)const{
	chainNode<T>*currentNode = firstNode;
	int index = 0;
	while(currentNode != nullptr && currentNode->element!=e){
		currentNode = currentNode->next;
		index++;
	}
	if(currentNode == nullptr)
	return -1;
	return index;
}

template<typename T>
void chain<T>::erase(int index){
	checkIndex(index);
	chainNode<T>*deleteNode = firstNode;
	if(index == 0){
		deleteNode = firstNode;
		firstNode = firstNode->next;
	}
	else{
		chainNode*p = firstNode;
		for(int i = 0; i < index - 1; i++)
			p = p->next;
			deleteNode = p->next;
			p->next = deleteNode->next;
	}
	listSize--;
	delete deleteNode;
}

template<typename T>
void chain<T>::insert(int index, const T&e){
	if(index < 0 || index>listSize){
		ostringstream s;
		s<<"index = "<<index<<", size = "<<listSize;
		throw illegalIndex(s.str());
	}
	if(index == 0)
		firstNode = new chainNode<T>(e,firstNode);
		else{
			chainNode<T>*p = firstNode;
			for(int i = 0; i < index - 1; i++)
				p = p->next;
			p-next = new chainNode<T>(e,p->next);
		}
		listSize++;
}

template<typename T>
void chain<T>::output(ostream &out)const{
	for(chainNode<T>* currentNode  = firstNode;
		currentNode !=nullptr;
		currentNode = currentNode->next)
		out<<currentNode->element<<" ";
	)
}
template<typename T>
ostream &operator<<(ostream& out,const chain<T>&list){
	list.output(out);
	return out;
}





