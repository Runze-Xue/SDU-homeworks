#pragma once
#ifndef _CITY_
#define _CITY_

#include<iostream>
#include"warrior.h"

class city {
public:
	static long long recordtime;//记录全局时间流向
	long long redselection = 0;//记录武士生成到哪一个了
	long long blueselection = 0;
	long long rednumber = 1;
	long long bluenumber = 1;
	long long redHP;//记录红方基地生命值
	long long blueHP;
	long long dragonHP;
	long long ninjiaHP;
	long long icemanHP;
	long long lionHP;
	long long wolfHP;
//	city()

};















#endif // _CITY_