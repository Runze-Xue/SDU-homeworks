#include<iostream>
#include<vector>
using namespace std;

int main(){
    char a[2][4] = {"abc", "123"};
    // a[0] = "abc";
    // a = {"abc""123"};
    vector<char**>v;
    v.push_back((char**)(a));
    printf("%s",v[0][0]);
    return 0;
}