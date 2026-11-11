#define _CRT_SECURE_NO_WARNINGS 
#include<iostream>
#include"DNode.h"
#include<limits.h>

using namespace std;

int main() {
	DNode<int> headleft(INT_MAX),headright(INT_MAX);
	DNode<int>* ptrleft = &headleft;
	DNode<int>* ptrright = &headright;
	int n = 0;
	cout << "输入的结点个数：" << "  ";
	scanf("%d", &n);
	cout << "输入结点值：";
	for (int i = 1; i <= n; i++) {
		int data; scanf("%d", &data);
		DNode<int> *Node1 = new DNode<int>(data);
		DNode<int>* Node2 = new DNode<int>(data);
		ptrleft->insertLeft(Node1);
		ptrleft = ptrleft->nextNodeLeft();
		ptrright->insertRight(Node2);
		ptrright = ptrright->nextNodeRight();
	}
	ptrleft = &headleft;
	cout << "想要删除左插入的链表中的元素：";
	int del; scanf("%d", &del);
	int g = 0;
	while (g<n) {
		if
			(ptrleft->data == del) {
			DNode<int>* ptrr = ptrleft;
			(ptrleft->nextNodeLeft())->insertRight(ptrright);
			ptrleft = ptrleft->nextNodeRight();
			ptrr->deleteNode();
		}
		else {
			ptrleft = ptrleft->nextNodeRight();
			cout << "ptrleft:" << ptrleft->data << endl;
		}
		g++;
	}
	/*cout << "输出左插入的链表： ";
	ptrleft = &headleft; ptrleft = ptrleft->nextNodeRight();
	ptrright = &headright; ptrright = ptrright->nextNodeRight();
	while (ptrleft != NULL) {
		cout << ptrleft->data << " ";
		ptrleft = ptrleft->nextNodeRight();
	}
	cout << endl;
	cout << "输出右插入的链表：";
	while (ptrright != NULL) {
		cout << ptrright->data << "  ";
		ptrright = ptrright->nextNodeRight();
	}
	cout << endl;
	ptrleft = &headleft;
	ptrright = &headright;
	while (ptrleft != NULL) {
		DNode<int>* pl = ptrleft;
		ptrleft = ptrleft->nextNodeRight();
		delete pl;
	}
	while (ptrright != NULL) {
		DNode<int>* p2 = ptrright;
		ptrright = ptrright->nextNodeRight();
		delete p2;
	}*/
	return 0;
}

