#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<easyx.h>
int main() {
	initgraph(660, 770);
	setbkcolor(WHITE);
	cleardevice();
	setbkmode(TRANSPARENT);
	circle(100, 100, 20);
	setfillcolor(BLUE);
	fillcircle(100, 150, 20);
	setfillcolor(GREEN);
	solidcircle(100, 200, 20);
	settextcolor(RED);
	settextstyle(50, 0, TEXT("––≤›"));
	outtextxy(100, 100, TEXT("–¶À¿Œ“¿±"));

	IMAGE m;
	loadimage(&m, TEXT("./Flower.BMP"),300,400);
	putimage(0, 0, &m);
	getchar();
	closegraph();
	return 0;
}