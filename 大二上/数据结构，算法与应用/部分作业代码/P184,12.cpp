//使用课本P183 程序8-5作为这pushNode和popNode两个函数的类
void LinkedStack::pushNode(chainNode* theNode){
	theNode->next = stackTop;
	stackTop = theNode;
	stackSize++;
}


chainNode LinkedStack::popNode(){
	if(stackSize == 0) throw stackEmpty();
	chainNode<T>*nextNode = stackTop->next;
	chainNode<T> temp = *(stackTop);
	delete stackTop;
	stackTop = nextNode;
	stackSize--; 
	return temp;
} 
