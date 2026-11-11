#include"client.h"
#include<iostream>
#include<tchar.h>
#include<string>
using namespace std;
CLIENT::CLIENT() {
	ClientNum++;
}
void CLIENT::ChangeServerName(const char change[100]) {
	for (int i = 0; i < 100; i++) {
		ServerName[i] = change[i];
	}
}
void CLIENT::ShowServeName() {
	std::cout <<"服务器名称为：" << ServerName << std::endl;
}
void CLIENT::getClientNumber() {
	cout << "服务器个数为：" << ClientNum << endl;
}