#include<stdio.h>
#include<easyx.h>
#include<string.h>
int location(short,short);
int locationx(short x, short y);
int locationy(short x, short y);
int roln; int linen; double startx, starty, abheight, ablength;
char answhite[50][50]; char ansblack[50][50];//用于记录黑白棋子的位置，实现胜利达成；
         //int ansline(char[][50]); //用于判断横向是否满足获胜条件
int answhitelineroll(int, int);
int ansblacklineroll(int, int);

int rectheight, rectlength;

int main() {
	memset(answhite, 0, sizeof(answhite));
	memset(ansblack, 0, sizeof(ansblack));

	initgraph(1200, 800, EX_SHOWCONSOLE);//easyx 绘制窗口 EX_SHOWCONSOLE

	HWND hwndeasyx = GetHWnd();

	//WNDCLASS wndclass;//定义变量wndclass 为WNDCLASS类型 ，方便后续窗口操作

	//wndclass.style = CS_HREDRAW | CS_VREDRAW;// 窗口操作，拖动窗口时实时绘制窗口的宽和高

	double height = getheight();//获取窗口高度
	double length = getwidth();//获取窗口长度

	setlinecolor(BLACK);
	//polyline();
	setbkcolor(BROWN);
	setbkmode(TRANSPARENT);
	cleardevice();
	setlinestyle(PS_SOLID, 5);
	//绘制棋盘开始

	rectheight = 19.0 / 20 * height;  //矩形边框的高度坐标
	rectlength = 19.0 / 20 * length;//矩形边框的长度坐标
	//绘制矩形边框
	startx = 1.0 / 20 * length;
	starty = 1.0 / 20 * height;
	rectangle(startx, starty, rectlength, rectheight);//绘制矩形棋盘框，四个参数分别对应左部 x 坐标，顶部 y 坐标，右部 x 坐标，底部 y 坐标
	//绘制竖条直线
	abheight = rectheight - starty;//矩形高度
	ablength = rectlength - startx;//矩形长度

	wchar_t s1[10]; wchar_t s2[10];
	InputBox(s1, 10, TEXT("请输入行数"));
	InputBox(s2, 10, TEXT("请输入列数"));
	linen = _wtoi(s1) - 1;  // 将输入转换为数字
	roln = _wtoi(s2) - 1;
	//roln是列的数量 ，linen是行的数量
	for (int i = 1; i < roln + 1; i++) {
		line(startx + i * ablength / roln, starty, startx + i * ablength / roln, rectheight);//参数分别为 起始点的x，y 终止点的x，y
	}

	//绘制横条直线
	for (int i = 1; i < linen + 1; i++) {
		line(startx, starty + i * abheight / linen, rectlength, starty + i * abheight / linen);
	}
	//绘制棋盘结束

	//鼠标信息的接收判断开始
	ExMessage msg; //ExMessage声明的变量 用于保存鼠标消息 msg中可以保存鼠标的左右按键消息，可以获取点击坐标消息，键盘消息等等
	while (true) {
		if (peekmessage(&msg, EX_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (location(msg.x, msg.y))  //判断是否在线交点附近
				{
					if (getfillcolor() == WHITE) setfillcolor(BLACK);
					else setfillcolor(WHITE);

					if(ansblack[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] ==0&& answhite[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0)
					solidcircle(locationx(msg.x, msg.y), locationy(msg.x, msg.y), (ablength / (3 * roln)) < abheight / (3 * linen) ? (ablength / (3 * roln)) : (abheight / (3 * linen)));

					if (getfillcolor() == BLACK) { answhite[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] = 1; }
					else ansblack[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] = 1;



					if (answhitelineroll((roln * locationx(msg.x, msg.y) / rectlength), (linen * locationy(msg.x, msg.y) / rectheight)))
					{
						printf("111");
					}
					if (ansblacklineroll((roln * locationx(msg.x, msg.y) / rectlength), (linen * locationy(msg.x, msg.y) / rectheight)))
					{
						printf("222");
					}
				}
				break;

			default:
				break;
			}
		}
	}
	//鼠标信息的接受判断结束


	MessageBox(NULL, TEXT("大家好！"), TEXT("I LOVE YOU "), MB_OK);
	getchar();
	closegraph();
}


//判断鼠标坐标是否在棋盘交点
int location(short x,short y ) {
	for (int i = 0; i < linen + 1; i++) {     
		for (int j = 0; j < roln + 1; j++) {   
			if(x< startx + j * ablength / roln +ablength/(3*roln) 
				&& x>startx + j * ablength / roln -ablength/(3*roln) 
				&& y< starty + i * abheight / linen + abheight/(3*linen)
				&& y>starty + i * abheight / linen - abheight / (3 * linen) )
			{
				return 1;
			}
		}

	}
	return 0;

}
//给予交点位置  x坐标位置
int locationx(short x, short y) {
	for (int i = 0; i < linen + 1; i++) {
		for (int j = 0; j < roln + 1; j++) {
			if (x< startx + j * ablength / roln + ablength / (3 * roln)
				&& x>startx + j * ablength / roln - ablength / (3 * roln)
				&& y< starty + i * abheight / linen + abheight / (3 * linen)
				&& y>starty + i * abheight / linen - abheight / (3 * linen))
			{
				return startx + j * ablength / roln;
			}
		}

	}
}
//给予交点位置 y坐标位置
int locationy(short x, short y) {
	for (int i = 0; i < linen + 1; i++) {
		for (int j = 0; j < roln + 1; j++) {
			if (x< startx + j * ablength / roln + ablength / (3 * roln)
				&& x>startx + j * ablength / roln - ablength / (3 * roln)
				&& y< starty + i * abheight / linen + abheight / (3 * linen)
				&& y>starty + i * abheight / linen - abheight / (3 * linen))
			{
				return starty + i * abheight / linen;
			}
		}

	}


}


  //胜利条件判断
  //判断落点行有没有满足条件的棋子
//判断落点有没有满足条件的白色棋子
int answhitelineroll(int x, int y) {
	
	for (int i = 0; i + 4 <= roln; i++) {
		int sum = 0;
		sum = answhite[i][y] + answhite[i + 1][y] + answhite[i + 2][y] + answhite[i + 3][y] + answhite[i + 4][y];
		if (sum == 5) return 1;
	}
	for(int i=0;i+4<=linen;i++)
	{
		int sum = 0;
		sum = answhite[x][i] + answhite[x][i + 1] + answhite[x][i + 2] + answhite[x][i + 3] + answhite[x][i + 4];
		if (sum == 5) return 1;
	}

	//下面是左下到右上的判断
	int xnw = x, ynw = y;
	while (xnw-4 >= 0 && xnw+4 <= roln && ynw-4 >= 0 && ynw+4 <= roln)
	{
		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw - 1][ynw - 1] + answhite[xnw - 2][ynw - 2] + answhite[xnw - 3][ynw - 3] + answhite[xnw - 4][ynw - 4];
		if (sum == 5) return 1;
		xnw--; ynw--;

	}
	xnw = x, ynw = y;
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw + 1] + answhite[xnw + 2][ynw + 2] + answhite[xnw + 3][ynw + 3] + answhite[xnw + 4][ynw + 4];
		if (sum == 5) return 1;
		xnw++; ynw++;

	}
	xnw = x, ynw = y;
	//下面是左上到右下的判断
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw - 1][ynw + 1] + answhite[xnw - 2][ynw + 2] + answhite[xnw - 3][ynw + 3] + answhite[xnw - 4][ynw + 4];
		if (sum == 5) return 1;
		xnw--; ynw++;

	}
	xnw = x, ynw = y;
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw - 1] + answhite[xnw + 2][ynw - 2] + answhite[xnw + 3][ynw - 3] + answhite[xnw + 4][ynw - 4];
		if (sum == 5) return 1;
		xnw++; ynw--;

	}
	return 0;
}

//判断落点黑色棋子
int ansblacklineroll(int x, int y) {

	for (int i = 0; i + 4 <= roln; i++) {
		int sum = 0;
		sum = ansblack[i][y] + ansblack[i + 1][y] + ansblack[i + 2][y] + ansblack[i + 3][y] + ansblack[i + 4][y];
		if (sum == 5) return 1;
	}

	for (int i = 0; i + 4 <= linen; i++)
	{
		int sum = 0;
		sum = ansblack[x][i] + ansblack[x][i + 1] + ansblack[x][i + 2] + ansblack[x][i + 3] + ansblack[x][i + 4];
		if (sum == 5) return 1;
	}

	//下面是左下到右上的判断
	int xnw = x, ynw = y;
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		int sum = 0;
		sum = ansblack[xnw][ynw] + ansblack[xnw - 1][ynw - 1] + ansblack[xnw - 2][ynw - 2] + ansblack[xnw - 3][ynw - 3] + ansblack[xnw - 4][ynw - 4];
		if (sum == 5) return 1;
		xnw--; ynw--; 

	}
	xnw = x, ynw = y;
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		
		int sum = 0;
		sum = ansblack[xnw][ynw] + ansblack[xnw + 1][ynw + 1] + ansblack[xnw + 2][ynw + 2] + ansblack[xnw + 3][ynw + 3] + ansblack[xnw + 4][ynw + 4];
		if (sum == 5) return 1;
		xnw++; ynw++;

	}
	xnw = x, ynw = y;
	//下面是左上到右下的判断
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		int sum = 0;
		sum = ansblack[xnw][ynw] + ansblack[xnw - 1][ynw + 1] + ansblack[xnw - 2][ynw + 2] + ansblack[xnw - 3][ynw + 3] + ansblack[xnw - 4][ynw + 4];
		if (sum == 5) return 1;
		xnw--; ynw++;

	}
	xnw = x, ynw = y;
	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	{
		int sum = 0;
		sum = ansblack[xnw][ynw] + ansblack[xnw + 1][ynw - 1] + ansblack[xnw + 2][ynw - 2] + ansblack[xnw + 3][ynw - 3] + ansblack[xnw + 4][ynw - 4];
		if (sum == 5) return 1;
		xnw++; ynw--;

	}
	return 0;
}
//胜利条件判断结束


