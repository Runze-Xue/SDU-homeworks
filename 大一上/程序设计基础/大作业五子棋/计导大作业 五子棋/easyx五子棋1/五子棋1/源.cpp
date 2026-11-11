#include<stdio.h>
#include<easyx.h>
#include<string.h>
int pve = 0, pvp = 0;//模式调用判断，当鼠标满足时条件时赋值，用以判断使用什么模式
int location(short,short); 
int locationx(short x, short y);
int locationy(short x, short y);
int roln; int linen;  int startx, starty, abheight, ablength; wchar_t s1[10]; wchar_t s2[10];
char answhite[50][50]; char ansblack[50][50];//用于记录黑白棋子的位置，实现胜利达成；
         //int ansline(char[][50]); //用于判断横向是否满足获胜条件
int answhitelineroll(int, int);
int ansblacklineroll(int, int);
int rectheight, rectlength;
int prelinen, preroln; char ret = 1;// ret标志是否选择了返回主菜单
char retme = 0;//retme标志是否
char memorynum[20][20][2]; int memorynumk = 1; // memorynum的前两个数代表着位置，最后一个数0是黑色，1是白色；
char retback = 0;

int main() {
	
	memset(memorynum, 0, sizeof(memorynum));
	while (true) {
		pvp = 0; pve = 0; ret = 0 ; retme = 0;
		cleardevice;
		memset(answhite, 0, sizeof(answhite));
		memset(ansblack, 0, sizeof(ansblack));
		//ret = 0; //将主菜单返回标志归零(归0)，用于下一次返回
		initgraph(1200, 800, EX_SHOWCONSOLE);//easyx 绘制窗口 EX_SHOWCONSOLE

		IMAGE img;//定义一张图片
		loadimage(&img, TEXT("./sdulib.jpg "), 1200, 800);//填充背景图片
		putimage(0, 0, &img);
		//绘制输出文字PVP还是PVE
		setbkmode(TRANSPARENT);
		settextstyle(80, 50, TEXT("正楷"));
		outtextxy(100, 500, TEXT("PVP"));
		outtextxy(100, 600, TEXT("PVE"));

		settextstyle(50, 30, TEXT("草书"));
		settextcolor(BLACK);
		outtextxy(400, 0, TEXT("五子棋激情对决"));

		settextstyle(40, 20, TEXT("正楷"));
		outtextxy(380, 750, TEXT("点击此处回放上一局信息"));
		
		
		HWND hwndeasyx = GetHWnd();

		double backlocationlength = getwidth();
		double backlocationheight = getheight();// 用于回放上一局对决  还未能使用

		double height = getheight();//获取窗口高度
		double length = getwidth();//获取窗口长度

		setlinecolor(BLACK);
		//polyline();
		setbkmode(TRANSPARENT);
		//cleardevice();
		setlinestyle(PS_SOLID, 5);
		//绘制棋盘开始
		setfillcolor(RGB(120, 220, 160));
		rectheight = 9.0 / 10 * height;  //矩形边框的高度坐标
		rectlength = 9.0 / 10 * length;//矩形边框的长度坐标
		//绘制矩形边框

		startx = 1.0 / 10 * length;
		starty = 1.0 / 10 * height;
		//准备好绘制参数后，先不绘制，先进行图形化设计，用来选择PVP或者PVE
		//鼠标相关函数

		ExMessage msgmod;//保存鼠标消息
		while (true) {
			if (pvp == 1 || pve == 1||retme==1) break;

			if (peekmessage(&msgmod, EX_MOUSE))
			{
				switch (msgmod.message)
				{
				case WM_LBUTTONDOWN:
					if (msgmod.x >= 100 && msgmod.x <= 300 && msgmod.y <= 600 && msgmod.y >= 500)
					{
						pvp = 1;
						break;
					}
					else if (msgmod.x > 100 && msgmod.x <= 300 && msgmod.y <= 700 && msgmod.y >= 600)
					{
						pve = 1;

						break;
					}
					if (msgmod.x >= 388 && msgmod.x <= 811 && msgmod.y >= 747 && msgmod.y <= 781)
					{
						retme = 1;

					}

				default:
					break;
				}
			}
		}
		char retmebk2 = 0;
		//选择了信息回放后的信息回放画面
		if (retme == 1) {

			cleardevice();
			putimage(0, 0, &img);
			fillrectangle(startx, starty, rectlength, rectheight);
			settextcolor(YELLOW);
			

			//	roln是列的数量 ，linen是行的数量
			for (int i = 1; i < preroln; i++) {
				line(startx + i * ablength / preroln, starty, startx + i * ablength / preroln, rectheight);//参数分别为 起始点的x，y 终止点的x，y
				
			}
			//line(startx + (roln) * ablength / roln, starty, startx + (roln) * ablength / roln, rectheight);
			//绘制横条直线
			for (int i = 1; i < prelinen; i++) {
				line(startx, starty + i * abheight / prelinen, rectlength, starty + i * abheight / prelinen);
			}
			for(int i=0;i<=preroln;i++)
			{ 
				for(int j=0;j<=prelinen;j++)
				{   //0是黑色，1是白色
					if (memorynum[i][j][0] >0) 
					{
						setfillcolor(BLACK);
						solidcircle(startx+i*ablength/preroln,starty+j*abheight/prelinen, (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
						//outtextxy(startx + i * ablength / preroln, starty + j * abheight / prelinen, TEXT("memorynum[i][j][0]"));
						settextcolor(RED);
						RECT r = { startx + i * ablength - (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)),starty + j * abheight / prelinen + (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)),startx + i * ablength + (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)),starty + j * abheight / prelinen - (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)) };
						drawtext(memorynum[i][j][0], &r, DT_SINGLELINE);
					}
					else if(memorynum[i][j][1]>0)
					{
						setfillcolor(WHITE);
						solidcircle(startx + i * ablength / preroln, starty + j * abheight / prelinen, (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
					
					}
				}
			}
			//	memorynum[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight][1] 
			ExMessage msgre;
			settextcolor(BLACK);
			settextstyle(40, 20, TEXT("正楷"));
			outtextxy(380, 750, TEXT("点击此处结束信息回放"));
			while (retme == 1) {
				if (peekmessage(&msgre, EX_MOUSE)) {

					switch (msgre.message){

				    case WM_LBUTTONDOWN: 
				//		printf("x:%d\n", msgre.x);
				//		printf("y:%d", msgre.y);
						if (msgre.x >= 393 && msgre.x <= 771 && msgre.y >= 749 && msgre.y <= 780)
						{

							retmebk2 = 1;
							retme = 0;
							break;
						}

						

					default:
						break;

					}
				}
			}



			
		}
		if (retmebk2 == 1) continue;


		//绘制对战所用棋盘
		cleardevice();
		putimage(0, 0, &img);
		fillrectangle(startx, starty, rectlength, rectheight);//绘制矩形棋盘框，四个参数分别对应左部 x 坐标，顶部 y 坐标，右部 x 坐标，底部 y 坐标
		//绘制竖条直线
		abheight = rectheight - starty;//矩形高度
		ablength = rectlength - startx;//矩形长度

		memset(s1, 0, sizeof(s1));    memset(s2, 0, sizeof(s2));
		InputBox(s1, 10, TEXT("请输入行数"));
		InputBox(s2, 10, TEXT("请输入列数"));

		linen = _wtoi(s1);  // 将输入转换为数字
		roln = _wtoi(s2);
		prelinen = linen - 1;
		preroln = roln - 1;
		

		//	roln是列的数量 ，linen是行的数量
		for (int i = 1; i < preroln; i++) {
			line(startx + i * ablength / preroln, starty, startx + i * ablength / preroln, rectheight);//参数分别为 起始点的x，y 终止点的x，y
		}
		//line(startx + (roln) * ablength / roln, starty, startx + (roln) * ablength / roln, rectheight);
		//绘制横条直线
		for (int i = 1; i < prelinen; i++) {
			line(startx, starty + i * abheight / prelinen, rectlength, starty + i * abheight / prelinen);
		}

		//绘制棋盘结束

		//鼠标信息的接收判断开始
		//pvp开始
		if (pvp == 1) {
			memset(memorynum, 0, sizeof(memorynum));
			ExMessage msg; //ExMessage声明的变量 用于保存鼠标消息 msg中可以保存鼠标的左右按键消息，可以获取点击坐标消息，键盘消息等等
			while (ret==0) {
				
				
				if (peekmessage(&msg, EX_MOUSE))
				{
				//	printf("x:%d\n", msg.x);
			//		printf("y:%d\n", msg.y);
					switch (msg.message)
					{
					case WM_LBUTTONDOWN:
						if (location(msg.x, msg.y))  //判断是否在线交点附近
						{
							//		printf(":x的坐标为：%d:\n", (locationx(msg.x, msg.y) - startx) * preroln / ablength);
							//			printf(":y的坐标为：%d:\n", (locationy(msg.x, msg.y) - starty) * prelinen / abheight);
							//			printf("%d\n", locationx(msg.x, msg.y));
							//			printf("%d\n", locationy(msg.x, msg.y));
							if (getfillcolor() == WHITE)
							{
								setfillcolor(BLACK);
								if (ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][int((locationy(msg.x, msg.y) - starty) * prelinen / abheight)] == 0 && answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && ((locationx(msg.x, msg.y) - startx) * preroln / ablength != -1) && ((locationy(msg.x, msg.y) - starty) * prelinen / abheight != -1))//if (ansblack[int((locationx(msg.x, msg.y) - startx) * preroln / ablength) / ablength][int((locationy(msg.x, msg.y) - starty) * prelinen / abheight)] == 0 && answhite[int((locationx(msg.x, msg.y) - startx) * preroln / ablength)][int ((locationy(msg.x, msg.y) - starty) * prelinen / abheight)] == 0)
								{	//绘制黑白棋子

									solidcircle(locationx(msg.x, msg.y), locationy(msg.x, msg.y), (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
									//记录黑白棋子落子顺序
									ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] = 1;
									memorynum[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight][0] = memorynumk;//0是黑色，1是白色
									memorynumk++;

								}
								else {
									setfillcolor(WHITE);


								}
							}
							else
							{
								setfillcolor(WHITE);
								if (ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && ((locationx(msg.x, msg.y) - startx) * preroln / ablength != -1) && ((locationy(msg.x, msg.y) - starty) * prelinen / abheight != -1))//if (ansblack[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0 && answhite[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0)
								{	//绘制黑白棋子
									solidcircle(locationx(msg.x, msg.y), locationy(msg.x, msg.y), (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
									//记录黑白棋子落子顺序
									answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] = 1;
									memorynum[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight][1] = memorynumk;
									memorynumk++;
								}
								else
								{
									setfillcolor(BLACK);


								}
							}

							//获胜判断
							if (answhitelineroll(((locationx(msg.x, msg.y) - startx) * preroln / ablength), ((locationy(msg.x, msg.y) - starty) * prelinen / abheight)))
							{
								
								if (MessageBox(hwndeasyx, TEXT("恭喜，白棋获胜，点击确定返回主页面！点击取消退出"), TEXT("对局结束"), MB_YESNO) == IDYES) {
									ret=1;
									break;
								}
								else
								{

									closegraph;
									return 0;
								}
							}

							if (ansblacklineroll(((locationx(msg.x, msg.y) - startx) * preroln / ablength), ((locationy(msg.x, msg.y) - starty) * prelinen / abheight)))//(ansblacklineroll((roln * locationx(msg.x, msg.y) / rectlength), (linen * locationy(msg.x, msg.y) / rectheight)))
							{
								if (MessageBox(hwndeasyx, TEXT("恭喜，黑棋获胜，点击取消退出！"), TEXT("对局结束"), MB_YESNO) == IDYES) {
									ret=1;
									
								}
								else {

									closegraph;
									return 0;
								}
							}
					







							break;
						}
				

					


					default:
						break;
					}
				}
			}
			//鼠标信息的接受判断结束  第二while结束
			//pvp结束


			//pve开始

		}
		else if (pve == 1) {
			
			}
			//pve结束
			









		}




	





MessageBox(NULL, TEXT("大家好！"), TEXT("I LOVE YOU "), MB_OK);
	getchar();
	closegraph();
}



//判断鼠标坐标是否在棋盘交点
int location(short x,short y ) {
	for (int i = 0; i < prelinen+1 ; i++) {     
		for (int j = 0; j < preroln+1; j++) {   
			if(x< startx + j * ablength / preroln +ablength/(3*preroln) 
				&& x>startx + j * ablength / preroln -ablength/(3*preroln) 
				&& y< starty + i * abheight / prelinen + abheight/(3*prelinen)
				&& y>starty + i * abheight / prelinen - abheight / (3* prelinen) )
			{
				return 1;
			}
		}

	}
	return NULL;

}
//给予交点位置  x坐标位置
int locationx(short x, short y) {
	for (int i = 0; i < linen+1 ; i++) {
		for (int j = 0; j < roln+1 ; j++) {
			if (x< startx + j * ablength / preroln + ablength / (3 * roln)
				&& x>startx + j * ablength / preroln - ablength / (3 * roln)
				&& y< starty + i * abheight / prelinen + abheight / (3 * linen)
				&& y>starty + i * abheight / prelinen - abheight / (3 * linen))
			{
				return 1+startx + j * ablength / preroln;
			}
		}

	}
}
//给予交点位置 y坐标位置
int locationy(short x, short y) {
	for (int i = 0; i < linen+1 ; i++) {
		for (int j = 0; j < roln+1 ; j++) {
			if (x< startx + j * ablength / preroln + ablength / (3 * roln)
				&& x>startx + j * ablength / preroln - ablength / (3* roln)
				&& y< starty + i * abheight / prelinen + abheight / (3 * linen)
				&& y>starty + i * abheight /prelinen - abheight / (3 * linen))
			{
				return 1+starty + i * abheight / prelinen;
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
	while (xnw > 0 && ynw <=linen) {
		xnw--; ynw++;
	}

	while (xnw + 4 <= roln && ynw -4 >=0 ) {

		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw - 1] + answhite[xnw + 2][ynw - 2] + answhite[xnw + 3][ynw - 3] + answhite[xnw + 4][ynw - 4];
		if (sum == 5)
		{
			
			return 1;
		}
		xnw++; ynw--;
	}



	//int xnw = x, ynw = y;
	//while (xnw-4 >= 0 && xnw+4 <= roln && ynw-4 >= 0 && ynw+4 <= roln)
	//{
	//	int sum = 0;
	//	sum = answhite[xnw][ynw] + answhite[xnw - 1][ynw - 1] + answhite[xnw - 2][ynw - 2] + answhite[xnw - 3][ynw - 3] + answhite[xnw - 4][ynw - 4];
//		if (sum == 5) return 1;
//		xnw--; ynw--;

//	}
	//xnw = x, ynw = y;
//	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
//	{
//		int sum = 0;
//		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw + 1] + answhite[xnw + 2][ynw + 2] + answhite[xnw + 3][ynw + 3] + answhite[xnw + 4][ynw + 4];
//		if (sum == 5) return 1;
//		xnw++; ynw++;

//	}
	
	//下面是左上到右下的判断
	xnw = x; ynw = y;
	while (xnw  > 0 && ynw  > 0) {
		xnw--; ynw--;
	}
	
	while (xnw + 4 <= roln && ynw + 4 <= linen) {
		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw + 1] + answhite[xnw + 2][ynw + 2] + answhite[xnw + 3][ynw + 3] + answhite[xnw + 4][ynw + 4];
		if (sum == 5) return 1;
		
		xnw++; ynw++;
	}
	//while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
	//{
//		int sum = 0;
	//	sum = answhite[xnw][ynw] + answhite[xnw - 1][ynw + 1] + answhite[xnw - 2][ynw + 2] + answhite[xnw - 3][ynw + 3] + answhite[xnw - 4][ynw + 4];
	//	if (sum == 5) return 1;
//		xnw--; ynw++;

//	}
//	xnw = x, ynw = y;
//	while (xnw - 4 >= 0 && xnw + 4 <= roln && ynw - 4 >= 0 && ynw + 4 <= roln)
//	{
//		int sum = 0;
//		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw - 1] + answhite[xnw + 2][ynw - 2] + answhite[xnw + 3][ynw - 3] + answhite[xnw + 4][ynw - 4];
//		if (sum == 5) return 1;
//		xnw++; ynw--;

//	}
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
	while (xnw > 0 && ynw <= linen) {
		xnw--; ynw++;
	}

	while (xnw + 4 <= roln && ynw - 4 >= 0) {

		int sum = 0;
		sum = ansblack[xnw][ynw] + ansblack[xnw + 1][ynw - 1] + ansblack[xnw + 2][ynw - 2] + ansblack[xnw + 3][ynw - 3] + ansblack[xnw + 4][ynw - 4];
		if (sum == 5)
		{

			return 1;
		}
		xnw++; ynw--;
	}

	//下面是左上到右下的判断
	xnw = x; ynw = y;
	while (xnw > 0 && ynw > 0) {
		xnw--; ynw--;
	}

	while (xnw + 4 <= roln && ynw + 4 <= linen) {
		int sum = 0;
		sum = ansblack[xnw][ynw] + ansblack[xnw + 1][ynw + 1] + ansblack[xnw + 2][ynw + 2] + ansblack[xnw + 3][ynw + 3] + ansblack[xnw + 4][ynw + 4];
		if (sum == 5) return 1;

		xnw++; ynw++;
	}
	
	return 0;
}
//胜利条件判断结束


