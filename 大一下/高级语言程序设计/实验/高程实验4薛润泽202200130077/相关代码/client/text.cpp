#include"client.h"
#include<iostream>
#include<string>
#include<tchar.h>
using namespace std;
int CLIENT::ClientNum = 0;
char CLIENT::ServerName[100]="unnamed";
int main() {
	CLIENT client1;
	string st1 = "firstchangename";
	CLIENT::ChangeServerName(st1.c_str());
	CLIENT::getClientNumber();
	CLIENT::ShowServeName();
	CLIENT client2;
	st1 = "secondchangename";
	CLIENT::getClientNumber();
	CLIENT::ChangeServerName(st1.c_str());
	CLIENT::ShowServeName();
	CLIENT client3;
	st1 = "thirdchangename";
	CLIENT::ChangeServerName(st1.c_str());
	CLIENT::ShowServeName();
	CLIENT::getClientNumber();
	return 0;
}