#include<iostream>
#include<string>
using namespace std;

int main(){
	string a="12345",b="abcde";
	a+=b;
	printf("%s",a.c_str());
	return 0;
} 
