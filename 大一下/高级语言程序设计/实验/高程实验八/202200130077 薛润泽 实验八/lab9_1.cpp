#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"node.h"
using namespace std;

int main() {
	int a[11];
	a[0] = 0;
	Node<int> *head=new Node<int>;
	Node<int>* cut = head;
	std::cout << "请输入10个数据" << endl;
	for (int i = 1; i <= 10; i++) {
		scanf("%d", &a[i]);
		cut -> next = new Node<int>(a[i]);
		cut = cut->next;
		if (i == 10) cut-> next = NULL;
	}
	std::cout << "按顺序输出链表中的数据:" << endl;
	Node<int>* show = head;
	while (show != NULL)
	{
		show = show->next;
		int showdata;
		if (show != NULL) {
			showdata = show->getData();
			std::cout << showdata << "  ";
		}
	}//1 2 2 3 2 2 2 2 4 5
	cout << endl;
	std::cout << "请输入要删除的数据" << endl;
	int dely = 0; scanf("%d", &dely);
	Node<int>* startleft = head;
	Node<int>* startright =  head->next;
	while (startleft!=NULL&&startright != NULL) {
		if (startright->getData() != dely)
			startleft = startleft->next, startright = startright->next;
		else{
			//startleft->next=startright;
			if (startright == NULL) break;
			while (startright->getData() == dely)
			{
				Node<int>* symbol = startright;
				startright = startright->next;
				delete symbol;
				if (startright == NULL) break;
			}
			startleft->next = startright;
			startleft = startleft->next;
			if (startright == NULL) break;
			startright = startright->next;
			}			
	}
	std::cout << "顺序输出链表剩余结点" << endl;
	show = head;
	while (show != NULL)
	{
		show = show->next;
		int showdata; 
			if (show != NULL){
				showdata = show->getData();
			std::cout << showdata << "  "; }
	}
	cout << endl;
	while (head != NULL) {
		Node<int>*temp = head;
		head = head->next;
		delete temp;
	}
	return 0;
}
