#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"resource.h"
#include<easyx.h>
#include<string.h>
#include<math.h>
#include<direct.h>
int pve = 0, pvp = 0;//模式调用判断，当鼠标满足时条件时赋值，用以判断使用什么模式
int location(short, short);
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

//下列为pve变量
int autoxy[50][50];   //用于记录各个位点的权值（方案一取最大权值）
int chessweightline(int x,int y);
int chessweightroll(int x,int y);
int chessweightuptodown(int x, int y);
int chessweightdowntoup(int x, int y);
FILE* fppvp = NULL;
FILE* fppve = NULL;
FILE* fpeve = NULL;
//char ansbanfake[50][50];



//计算行权值 //计算列权值    //计算左上到右下的权值 //计算左下到右上的权值
// 权值规定  未落子权值为1，白子权值为1.2， 黑子权值为1.1  全列行计算

int main() {
	char Path[256];
	
	memset(memorynum, 0, sizeof(memorynum));
	while (true) {
		
		pvp = 0; pve = 0; ret = 0; retme = 0;
		cleardevice;
		memset(answhite, 0, sizeof(answhite));
		memset(ansblack, 0, sizeof(ansblack));

		memset(autoxy, 0, sizeof(autoxy));
		//ret = 0; //将主菜单返回标志归零(归0)，用于下一次返回
		initgraph(1200, 800);//easyx 绘制窗口 EX_SHOWCONSOLE
		
		IMAGE img;//定义一张图片
		loadimage(&img,L"mysource", MAKEINTRESOURCE(IDR_MYSOURCE1), 1200, 800);//填充背景图片("./sdulib.jpg ")
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
			if (pvp == 1 || pve == 1 || retme == 1) break;

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
			for (int i = 0; i <= preroln; i++)
			{
				for (int j = 0; j <= prelinen; j++)
				{   //0是黑色，1是白色
					if (memorynum[i][j][0] > 0)
					{
						setfillcolor(BLACK);
						solidcircle(startx + i * ablength / preroln, starty + j * abheight / prelinen, (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
						//outtextxy(startx + i * ablength / preroln, starty + j * abheight / prelinen, TEXT("memorynum[i][j][0]"));
						settextcolor(RED);
						RECT r = { startx + i * ablength - (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)),starty + j * abheight / prelinen + (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)),startx + i * ablength + (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)),starty + j * abheight / prelinen - (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)) };
						drawtext(memorynum[i][j][0], &r, DT_SINGLELINE);
					}
					else if (memorynum[i][j][1] > 0)
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

					switch (msgre.message) {

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
		setfillcolor(WHITE);//先落黑子
		if (pvp ==1) {
			fppvp = fopen("pvp.txt", "wt+");
		}
		if (pve == 1) {
			fppve = fopen("pve.txt", "wt+");
		}

		if (pvp == 1) {
			setfillcolor(BLUE);
			fillrectangle(450, 5, 700, 80);
			settextstyle(50, 30, TEXT("草书"));
			settextcolor(WHITE);
			outtextxy(450, 10, TEXT("该白子了"));
			setfillcolor(BLACK);
			memset(memorynum, 0, sizeof(memorynum));
			ExMessage msg; //ExMessage声明的变量 用于保存鼠标消息 msg中可以保存鼠标的左右按键消息，可以获取点击坐标消息，键盘消息等等
			while (ret == 0) {


				if (peekmessage(&msg, EX_MOUSE))
				{
					//	printf("x:%d\n", msg.x);
				//		printf("y:%d\n", msg.y);
					switch (msg.message)
					{
					case WM_LBUTTONDOWN:
						if (location(msg.x, msg.y))  //判断是否在线交点附近
						{
						
							if (getfillcolor() == WHITE)
							{
								setfillcolor(BLUE);
								fillrectangle(450, 5, 700, 80);
								settextstyle(50, 30, TEXT("草书"));
								settextcolor(WHITE);
								outtextxy(450, 10, TEXT("该白子了"));
								setfillcolor(BLACK);
								if (ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][int((locationy(msg.x, msg.y) - starty) * prelinen / abheight)] == 0 && answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && ((locationx(msg.x, msg.y) - startx) * preroln / ablength != -1) && ((locationy(msg.x, msg.y) - starty) * prelinen / abheight != -1))//if (ansblack[int((locationx(msg.x, msg.y) - startx) * preroln / ablength) / ablength][int((locationy(msg.x, msg.y) - starty) * prelinen / abheight)] == 0 && answhite[int((locationx(msg.x, msg.y) - startx) * preroln / ablength)][int ((locationy(msg.x, msg.y) - starty) * prelinen / abheight)] == 0)
								{	//绘制黑白棋子

									solidcircle(locationx(msg.x, msg.y), locationy(msg.x, msg.y), (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
									//记录黑白棋子落子顺序
									ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] = 1;
									memorynum[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight][0] = memorynumk;//0是黑色，1是白色
									memorynumk++;
									char wx[3]; char wy[3]; 
									memset(wx, NULL, sizeof(wx));
									memset(wy, NULL, sizeof(wy));
									
									if(((locationx(msg.x, msg.y) - startx) * preroln / ablength<10))
									wx[0] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength)+'0';
									else if (((locationx(msg.x, msg.y) - startx) * preroln / ablength >= 10)) {
										wx[0]= (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength)/10+'0';
										wx[1] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength % 10) + '0';
									}
									if((locationy(msg.x, msg.y) - starty) * prelinen / abheight<10)
								     wy[0] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight)+'0';
									else if ((locationy(msg.x, msg.y) - starty) * prelinen / abheight >= 10) {
										wy[0] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight)/10+'0';
										wy[1] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight%10) + '0';
									}
									fputs("+x", fppvp);
									fputs(wx, fppvp);
									fputs("y", fppvp);
									fputs(wy, fppvp);
									
									printf("888");

								}
								else {
									setfillcolor(WHITE);


								}
							}
							else
							{
								setfillcolor(BLUE);
								fillrectangle(450, 5, 700, 80);
								settextstyle(50, 30, TEXT("草书"));
								settextcolor(WHITE);
								outtextxy(450, 10, TEXT("该黑子了"));
								setfillcolor(BLACK);
								setfillcolor(WHITE);
								if (ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && ((locationx(msg.x, msg.y) - startx) * preroln / ablength != -1) && ((locationy(msg.x, msg.y) - starty) * prelinen / abheight != -1))//if (ansblack[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0 && answhite[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0)
								{	//绘制黑白棋子
									solidcircle(locationx(msg.x, msg.y), locationy(msg.x, msg.y), (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
									//记录黑白棋子落子顺序
									answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] = 1;
									memorynum[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight][1] = memorynumk;
									memorynumk++;
									char wx[3]; char wy[3];
									memset(wx, NULL, sizeof(wx));
									memset(wy, NULL, sizeof(wy));

									if (((locationx(msg.x, msg.y) - startx) * preroln / ablength < 10))
										wx[0] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength) + '0';
									else if (((locationx(msg.x, msg.y) - startx) * preroln / ablength >= 10)) {
										wx[0] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength) / 10 + '0';
										wx[1] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength % 10) + '0';
									}
									if ((locationy(msg.x, msg.y) - starty) * prelinen / abheight < 10)
										wy[0] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight) + '0';
									else if ((locationy(msg.x, msg.y) - starty) * prelinen / abheight >= 10) {
										wy[0] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight) / 10 + '0';
										wy[1] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight % 10) + '0';
									}
									fputs("-x", fppvp);
									fputs(wx, fppvp);
									fputs("y", fppvp);
									fputs(wy, fppvp);
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
									ret = 1; fclose(fppvp);
									break;
								}
								else
								{
									fclose(fppvp);
									closegraph;
									return 0;
								}
							}

							if (ansblacklineroll(((locationx(msg.x, msg.y) - startx) * preroln / ablength), ((locationy(msg.x, msg.y) - starty) * prelinen / abheight)))//(ansblacklineroll((roln * locationx(msg.x, msg.y) / rectlength), (linen * locationy(msg.x, msg.y) / rectheight)))
							{
								if (MessageBox(hwndeasyx, TEXT("恭喜，黑棋获胜，点击取消退出！"), TEXT("对局结束"), MB_YESNO) == IDYES) {
									ret = 1; fclose(fppvp);
									break;

								}
								else {
									fclose(fppvp);
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
			setfillcolor(BLUE);
			fillrectangle(450, 5, 700, 80);
			settextstyle(50, 30, TEXT("草书"));
			settextcolor(WHITE);
			outtextxy(450, 10, TEXT("人机对战！"));
			setfillcolor(BLACK);
			memset(memorynum, 0, sizeof(memorynum));
			ExMessage msg; //ExMessage声明的变量 用于保存鼠标消息 msg中可以保存鼠标的左右按键消息，可以获取点击坐标消息，键盘消息等等
			ret = 0;
			while (ret == 0) {   //ret 是对战结束后的标志0/1
	
				int sumai = 0; int kmaxai = 0; 
				//ai 权值计算判断
				int kx = 0; int ky = 0;//kx ,ky,用来记录拥有最大权值的落点坐标
				for (int i = 0; i <= preroln; i++) {
				
					for (int j = 0; j <= prelinen; j++) {
						
						if (answhite[i][j] == 1 || ansblack[i][j] == 1) continue;//如果已经落子，则跳过该点
						sumai = chessweightline(i, j)+ chessweightroll(i, j)+ chessweightdowntoup(i, j) +chessweightuptodown(i, j);
					
						if (sumai > kmaxai) {
							kx = i; ky = j;
							kmaxai = sumai;
						}
						sumai = 0;
					}
				}
				printf(":%d:", chessweightline(kx, ky)); printf(":%d:", chessweightroll(kx, ky)); printf(":%d:", chessweightdowntoup(kx, ky)); printf(":%d:", chessweightuptodown(kx, ky));
				printf("x:%d\n", kx);
				printf("y:%d\n", ky);
				setfillcolor(BLACK);
				solidcircle(1+startx + kx * ablength / preroln ,  1+starty + ky * abheight / prelinen , (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
			//	1 + startx + j * ablength / preroln;
				ansblack[kx][ky] = 1;
				memorynum[kx][ky][0] = memorynumk;
			//	ansbanfake[kx][ky] = 1;
				memorynumk++;
				
				char wx[3]; char wy[3];
				memset(wx, NULL, sizeof(wx));
				memset(wy, NULL, sizeof(wy));

				if (kx<10)
					wx[0] = (char)(kx) + '0';
				else if (kx>= 10){
					wx[0] = (char)(kx) / 10 + '0';
					wx[1] = (char)(kx% 10) + '0';
				}
				if (ky<10)
					wy[0] = (char)(ky) + '0';
				else if (ky >= 10) {
					wy[0] = (char)(ky) / 10 + '0';
					wy[1] = (char)(ky% 10) + '0';
				}
				fputs("+x", fppve);
				fputs(wx, fppve);
				fputs("y", fppve);
				fputs(wy, fppve);
				
				
				if (ansblacklineroll(kx, ky) ) //(ansblacklineroll((roln * locationx(msg.x, msg.y) / rectlength), (linen * locationy(msg.x, msg.y) / rectheight)))
				{
					if (MessageBox(hwndeasyx, TEXT("恭喜，黑棋获胜，点击取消退出！"), TEXT("对局结束"), MB_YESNO) == IDYES) {
						ret = 1;
						fclose(fppve);
						break;
					}
					else {

						closegraph; fclose(fppve);
							return 0;
					}
				}
				//人操作；
			
				int jet = 0;
				while (jet == 0) {
					if (peekmessage(&msg, EX_MOUSE))
					{

						switch (msg.message)
						{
						
						case WM_LBUTTONDOWN:
							if (location(msg.x, msg.y))  //判断是否在线交点附近
							{

								jet = 1;//取消无限循环；

								setfillcolor(WHITE);
								if (ansblack[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] == 0 && ((locationx(msg.x, msg.y) - startx) * preroln / ablength != -1) && ((locationy(msg.x, msg.y) - starty) * prelinen / abheight != -1))//if (ansblack[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0 && answhite[roln * locationx(msg.x, msg.y) / rectlength][linen * locationy(msg.x, msg.y) / rectheight] == 0)
								{	//绘制黑白棋子
									solidcircle(locationx(msg.x, msg.y), locationy(msg.x, msg.y), (ablength / (2.5 * roln)) < abheight / (2.5 * linen) ? (ablength / (2.5 * roln)) : (abheight / (2.5 * linen)));
									//记录黑白棋子落子顺序
									answhite[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] = 1;
								//	ansbanfake[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight] = 2;
									memorynum[(locationx(msg.x, msg.y) - startx) * preroln / ablength][(locationy(msg.x, msg.y) - starty) * prelinen / abheight][1] = memorynumk;
									memorynumk++;
									char wx2[3]; char wy2[3];
									memset(wx2, NULL, sizeof(wx2));
									memset(wy2, NULL, sizeof(wy2));

									if (((locationx(msg.x, msg.y) - startx) * preroln / ablength < 10))
										wx2[0] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength) + '0';
									else if (((locationx(msg.x, msg.y) - startx) * preroln / ablength >= 10)) {
										wx2[0] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength) / 10 + '0';
										wx2[1] = (char)((locationx(msg.x, msg.y) - startx) * preroln / ablength % 10) + '0';
									}
									if ((locationy(msg.x, msg.y) - starty) * prelinen / abheight < 10)
										wy2[0] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight) + '0';
									else if ((locationy(msg.x, msg.y) - starty) * prelinen / abheight >= 10) {
										wy2[0] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight) / 10 + '0';
										wy2[1] = (char)((locationy(msg.x, msg.y) - starty) * prelinen / abheight % 10) + '0';
									}
									fputs("-x", fppve);
									fputs(wx2, fppve);
									fputs("y", fppve);
									fputs(wy, fppve);

								}
								else
								{
									setfillcolor(BLACK);
								}
							}

						default:
							break;
						}
					}
				}
				
						//获胜判断
						if (answhitelineroll(((locationx(msg.x, msg.y) - startx) * preroln / ablength), ((locationy(msg.x, msg.y) - starty) * prelinen / abheight)))
						{

							if (MessageBox(hwndeasyx, TEXT("恭喜，白棋获胜，点击确定返回主页面！点击取消退出"), TEXT("对局结束"), MB_YESNO) == IDYES) {
								ret = 1; fclose(fppve);
								break;
							}
							else
							{
								fclose(fppve);
								closegraph;
								return 0;
							}
						}
						printf("%d", ret);
			}
			//鼠标信息的接受判断结束  第二while结束
		}
		//pve结束
		
	}
	MessageBox(NULL, TEXT("大家好！"), TEXT("I LOVE YOU "), MB_OK);
	getchar();
	closegraph();
}

//判断鼠标坐标是否在棋盘交点
int location(short x, short y) {
	for (int i = 0; i < prelinen + 1; i++) {
		for (int j = 0; j < preroln + 1; j++) {
			if (x< startx + j * ablength / preroln + ablength / (3 * preroln)
				&& x>startx + j * ablength / preroln - ablength / (3 * preroln)
				&& y< starty + i * abheight / prelinen + abheight / (3 * prelinen)
				&& y>starty + i * abheight / prelinen - abheight / (3 * prelinen))
			{
				return 1;
			}
		}
	}
	return NULL;
}
//给予交点位置  x坐标位置
int locationx(short x, short y) {
	for (int i = 0; i < linen + 1; i++) {
		for (int j = 0; j < roln + 1; j++) {
			if (x< startx + j * ablength / preroln + ablength / (3 * roln)
				&& x>startx + j * ablength / preroln - ablength / (3 * roln)
				&& y< starty + i * abheight / prelinen + abheight / (3 * linen)
				&& y>starty + i * abheight / prelinen - abheight / (3 * linen))
			{
				return 1 + startx + j * ablength / preroln;
			}
		}
	}
}
//给予交点位置 y坐标位置
int locationy(short x, short y) {
	for (int i = 0; i < linen + 1; i++) {
		for (int j = 0; j < roln + 1; j++) {
			if (x< startx + j * ablength / preroln + ablength / (3 * roln)
				&& x>startx + j * ablength / preroln - ablength / (3 * roln)
				&& y< starty + i * abheight / prelinen + abheight / (3 * linen)
				&& y>starty + i * abheight / prelinen - abheight / (3 * linen))
			{
				return 1 + starty + i * abheight / prelinen;
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
	for (int i = 0; i + 4 <= linen; i++)
	{
		int sum = 0;
		sum = answhite[x][i] + answhite[x][i + 1] + answhite[x][i + 2] + answhite[x][i + 3] + answhite[x][i + 4];
		if (sum == 5) return 1;
	}

	//下面是左下到右上的判断
	int xnw = x, ynw = y;
	while (xnw > 0 && ynw <= linen) {
		xnw--; ynw++;
	}

	while (xnw + 4 <= roln && ynw - 4 >= 0) {

		int sum = 0;
		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw - 1] + answhite[xnw + 2][ynw - 2] + answhite[xnw + 3][ynw - 3] + answhite[xnw + 4][ynw - 4];
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
		sum = answhite[xnw][ynw] + answhite[xnw + 1][ynw + 1] + answhite[xnw + 2][ynw + 2] + answhite[xnw + 3][ynw + 3] + answhite[xnw + 4][ynw + 4];
		if (sum == 5) return 1;

		xnw++; ynw++;
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


//机器落子函数  autoxy


int chessweightline(int x,int y)
{

	int x1 = x; int y1 = y; int sum = 0; int sym = 0;
	int countleft = 0;
	int countright = 0;
	while (x1 > 0) x1--;
	while (x1 <= roln) {
		if (ansblack[x1][y1] == 0 && answhite[x1][y1] == 0) {
			sum = sum + 2;
		}
		x1++;
	}
	x1 = x; y1 = y;
	while (x1 > 0) {
		countleft++;   x1--;
	}
	x1 = x;
	if (countleft > 4) countleft = 4;
	while (x1 < preroln) {
		countright++; x1++;
	}
	x1 = x;
	if (countright > 4) countright = 4;
	//特例
	int loctx1 = 0; int loctx2 = 100000;
	for (int i = -countleft; i <= 0; i++) {
		if (answhite[x1+i][y1] == 1) {
			if (countright - i <= 4) return 0;
			if (x1 + i > loctx1) loctx1 = x1 + i;
		}
	}
	for (int i = 0; i <= countright; i++) {
		if (answhite[x1 + i][y1] == 1) {
			if (i + countleft <= 4) return 0;
			if (x1 + i < loctx2) loctx2 = x1 + i;
		}
	}
	if (loctx2-loctx1<6) return 0;
	//特例结束
	//普遍开始
	int countleftr = 0; int countrightr = 0;
	x1 = x; y1 = y;
	while (x1 > 0) {
		countleftr++;
		x1--;
	}
	if (countleftr > 4) countleftr = 4;
	x1 = x;
	while (x1 < preroln) {
		countrightr++;
		x1++;
	}
	if (countrightr > 4) countrightr = 4;
	x1 = x; y1 = y;
	sym = 0;
	//尝试计算全部和
	int fusym = 0;
	if (ansblack[x][y] == 0 && answhite[x][y] == 0) {
		ansblack[x][y] = 1; fusym = 1;
	}
	for (int i = -countleftr; i+4 <= countrightr; i++) {
		int max = 0; int maxsym = 0;
		for (int k = 0; k <= 4; k++) {

			if (answhite[x1 + i + k][y1] == 0 && ansblack[x1 + i + k][y1] == 0) maxsym =maxsym + 2;
			
		}
		if (maxsym > max) max =maxsym ;
		maxsym = 0;
		for (int k = 0; k <= 4; k++) {
			 if (answhite[x1 + i + k][y1] == 1 && ansblack[x1 + i + k][y1] == 0)maxsym = maxsym + 14;
		}
		if (maxsym > max) max = maxsym;
		maxsym = 0;
		for (int k = 0; k <= 4; k++) {
		 if (answhite[x1 + i + k][y1] == 0 && ansblack[x1 + i + k][y1] == 1) maxsym = maxsym + 15;
		}
		if (maxsym > max) max = maxsym;
		sym = sym + max;

			//相邻两个的增量加和
			if ((x1 + i + 4 <= preroln) && ansblack[x1 + i][y1] == 1 && ansblack[x1 + i + 1][y1] == 1 && ansblack[x1 + i + 2][y1] == 1 && ansblack[x1 + i + 3][y1]==1 && ansblack[x1 + i + 4][y1] == 1) sym = sym+2000000;
			else		if ((x1 + i + 3 <= preroln) && answhite[x1 + i][y1] == 1 && answhite[x1 + i + 1][y1] == 1 && answhite[x1 + i + 2][y1] == 1 && answhite[x1 + i + 3][y1] == 1) sym = sym + 300000;
			else		if ((x1 + i  + 3 <= preroln) && ansblack[x1 + i ][y1] == 1 && ansblack[x1 + i  + 1][y1] == 1 && ansblack[x1 + i  + 2][y1] == 1&&ansblack[x1+i+3][y1]==1) sym = sym + 70000;
			else		 if ((x1 + i  + 3 <= preroln) && answhite[x1 + i ][y1] == 1 && ansblack[x1 + i  + 1][y1] == 1&& ansblack[x1 + i  + 2][y1] == 1 && ansblack[x1 + i  + 3][y1] == 1) sym = sym + 0;
			else		 if ((x1 + i  + 3 <= preroln) && ansblack[x1 + i ][y1] == 1 && ansblack[x1 + i  + 1][y1] == 1 && ansblack[x1 + i  + 2][y1] == 1 && answhite[x1 + i  + 3][y1] == 1) sym = sym + 0;
			else		 if ((x1 + i  + 2 <= preroln) && answhite[x1 + i ][y1] == 1 && ansblack[x1 + i  + 1][y1] == 1 && ansblack[x1 + i  + 2][y1] == 1) sym = sym + 0;
			else		 if ((x1 + i  + 2 <= preroln) && answhite[x1 + i ][y1] == 1 && answhite[x1 + i  + 1][y1] == 1 && ansblack[x1 + i + 2][y1] == 1) sym = sym + 1600;
			else		 if ((x1 + i  + 2 <= preroln) && ansblack[x1 + i ][y1] == 1 && ansblack[x1 + i  + 1][y1] == 1 && answhite[x1 + i  + 1][y1] == 1) sym = sym + 0;
			else		 if ((x1 + i  + 3 <= preroln) && ansblack[x1 + i ][y1] == 0 && ansblack[x1 + i  + 1][y1] == 1 &&  ansblack[x1 + i  + 2][y1] == 1 && ansblack[x1 + i + 3][y1] == 0)sym = sym+ 500;
			else	 if((x1 + i  + 2 <= preroln) && ansblack[x1 + i ][y1] == 1 && ansblack[x1 + i  + 1][y1] == 1&&ansblack[x1+i+2][y1]==0) sym = sym + 120;
			else		  if ((x1 + i + 2 <= preroln) && ansblack[x1 + i][y1] == 0 && ansblack[x1 + i + 1][y1] == 1 && ansblack[x1 + i + 2][y1] == 1) sym = sym + 120;

			 if (abs(sym) > sum) sum = abs(sym);
		
		//sum = abs(sum);
	}
	if(fusym==1)
	ansblack[x][y] = 0;
	return sum;
}

int chessweightroll(int x, int y) { 

	int x1 = x; int y1 = y; int sum = 0; int sym = 0;
	while ( y1 > 0) {
		 y1--;
	}
	while (y1 <=prelinen) {
		if (answhite[x1][y1] == 0 && ansblack[x1][y1] == 0) {
			sum = sum + 2;
		}
		y1++;
	}
	x1 = x; y1 = y;
	int countup = 0;
	int countdown = 0;
	while (y1 > 0) {
		countup++;   y1--;
	}
	y1 = y;
	if (countup > 4) countup = 4;
	while (y1 < prelinen) {
		countdown++; y1++;
	}
	y1 = y;
	if (countdown > 4) countdown = 4;
	//特例
	int locty1 = 0; int locty2 = 100000;
	for (int i = -countup; i <= 0; i++) {
		if (answhite[x1][y1+i] == 1) {
			if (countdown - i <= 4) return 0;
			if (x1 + i > locty1) locty1 = y1 + i;
		}
	}
	for (int i = 0; i <= countdown; i++) {
		if (answhite[x1][y1+i] == 1) {
			if (i + countup <= 4) return 0;
			if (y1 + i < locty2) locty2 = y1 + i;
		}
	}
	if (locty2 - locty1 < 6) return 0;
	//特例结束
	//普遍开始
	int countupr = 0; int countdownr = 0;
	x1 = x; y1 = y;
	while (y1 > 0) {
		countupr++;
		y1--;
	}
	if (countupr > 4) countupr = 4;
	y1 = x;
	while (y1 < prelinen) {
		countdownr++;
		y1++;
	}
	if (countdownr > 4) countdownr = 4;
	x1 = x; y1 = y;
	sym = 0;
	//尝试计算全部和
	int fusym = 0;
	if (ansblack[x][y] == 0 && answhite[x][y] == 0) {
		ansblack[x][y] = 1; fusym = 1;
	}
	for (int i = -countupr; i + 4 <= countdownr; i++) {
		int cbl = 0, cwt = 0;//记录五个子中黑白棋子数量
		for (int k = 0; k <= 4; k++) {
			if (answhite[x1][y1 + i + k] == 0 && ansblack[x1][y1 + i + k] == 0) sym = sym + 2;
			else if (answhite[x1][y1 + i + k] == 1 && ansblack[x1][y1 + i + k] == 0)
			{
				sym = sym - 14; cwt++;
			}
			else if (answhite[x1][y1 + i + k] == 0 && ansblack[x1][y1 + i + k] == 1)
			{
				sym = sym + 15; cbl++;
			}
			//相邻两个的增量加和
		}		
		if ((y1 + i + 4 <= prelinen) && ansblack[x1][y1 + i] == 1 && ansblack[x1][y1 + i + 1] == 1 && ansblack[x1][y1 + i + 2] == 1 && ansblack[x1][y1 + i + 3] == 1&&ansblack[x1][y1+i+4]==1) sym = sym + 2000000;
				else	if ((y1 + i + 3 <= prelinen) && answhite[x1][y1 + i] == 1 && answhite[x1][y1 + i + 1] == 1 && answhite[x1][y1 + i + 2] == 1 && answhite[x1][y1 + i + 3] == 1) sym = sym + 16000;
				else	if ((y1 + i + 3 <= prelinen) && ansblack[x1][y1 + i] == 1 && ansblack[x1][y1 + i + 1] == 1 && ansblack[x1][y1 + i + 2] == 1 && ansblack[x1][y1 + i + 3] == 1) sym = sym + 10000;
				else	if ((y1 + i + 3 <= prelinen && answhite[x1][y1 + i ] == 1 && ansblack[x1][y1 + i  + 1] == 1 && ansblack[x1][y1 + i  + 2] == 1 && ansblack[x1][y1 + i  + 3] == 1)) sym = sym + 0;
				else	if ((y1 + i  + 3 <= prelinen && ansblack[x1][y1 + i ] == 1 && ansblack[x1][y1 + i  + 1] == 1 && ansblack[x1][y1 + i  + 2] == 1 && answhite[x1][y1 + i  + 3] == 1)) sym = sym + 0;
				else if ((y1 + i  + 2 <= prelinen) && ansblack[x1][y1 + i ] == 1 && ansblack[x1][y1 + i  + 1] == 1 && ansblack[x1][y1 + i  + 2] == 1&&cwt==0) sym = sym +2000;
				else	if ((y1 + i  + 3 <= prelinen) && ansblack[x1][y1 + i ] == 1 && answhite[x1][y1 + i  + 1] == 1 && answhite[x1][y1 + i  + 2] == 1 && answhite[x1][y1 + i  + 3] == 1) sym = sym +2000;
			else	if ((y1 + i + 3 <= prelinen) && ansblack[x1][y1 + i ] == 2 && ansblack[x1][y1 + i  + 1] == 2&& ansblack[x1][y1 + i  + 2] == 2 && ansblack[x1][y1 + i  + 3] == 1) sym = sym +0;
			else	if ((y1 + i  + 2 <= prelinen) && ansblack[x1][y1 + i ] == 2 && ansblack[x1][y1 + i  + 1] == 2 && ansblack[x1][y1 + i + 2] == 2) sym = sym + 1600;
			else	if ((y1 + i  + 1 <= prelinen) && ansblack[x1][y1 + i ] == 1 && ansblack[x1][y1 + i  + 1] == 1) sym = sym + 120;

				if (abs(sym) > sum)  sum = abs(sym);
				sym = 0;
		
	//	if(abs(sym)>sum)  sum=abs(sym);
	//	sym = 0;
	}
	if(fusym==1)
	ansblack[x][y] = 0;
	return sum;

}

int chessweightuptodown(int x, int y) {
	
	int x1 = x; int y1 = y; int sum = 0; int sym = 0;
	while (x1 > 0 && y1 >0) {
		x1--; y1--;
	}

	//循环停止时x1，y1至少有一个为0；
	while (x1 <= preroln && y1 <=prelinen) {
		if (answhite[x1][y1] == 0 && ansblack[x1][y1] == 0) {
			sym = sym + 2;
		}
		x1++; y1++;
	}

	if (sym > sum) sum = sym;
	sym = 0;
	//无法围成五子的特例
	x1 = x; y1 = y;
	int countminorx = 0;
	while (x1 > 0 && y1 >0) {
		countminorx++;
		x1--; y1--;
	}
	x1 = x; y1 = y;
	if (countminorx > 4) countminorx = 4;
	int countplusy = 0;
	while (x1 < preroln && y1<prelinen) {
		countplusy++;
		x1++; y1++;
	}
	x1 = x; y1 = y;
	if (countplusy > 4) countplusy = 4;
	int countsmall = 0;
	int countbig = 1000000;

	for (int i = -countminorx; i <= 0; i++) {
		if (answhite[x1 + i][y1 + i] == 1) {
			if (countplusy - i <= 4) return 0;
			if (-i > countsmall) countsmall = -i;
		}
	}
	for (int i = 0; i <= countplusy; i++) {
		if (answhite[x1 + i][y1+i] == 1) {
			if (countminorx + i <= 4) return 0;
			if (i < countbig) countbig = i;
		}
	}
	if (countbig-countsmall<6) return 0;
	//特例结束；

	x1 = x; y1 = y;
	int countminorxr = 0;
	while (x1 > 0 && y1 < prelinen) {
		countminorxr++;
		x1--; y1++;
	}
	x1 = x; y1 = y;
	if (countminorxr > 4) countminorxr = 4;
	int countplusyr = 0;
	while (x1 < preroln && y1>0) {
		countplusyr++;
		x1++; y--;
	}
	x1 = x; y1 = y;
	if (countplusyr > 4) countplusyr = 4;

	int fusym = 0;
	x1 = x; y1 = y;
	if (ansblack[x1][y1] == 0 && answhite[x1][y1] == 0) {
		ansblack[x1][y1] = 1; fusym = 1;
	}
	//常规计算
	sym = 0; //int countwhite = 0; int countblack = 0;
	for (int i = -countminorxr; i + 4 <= countplusyr; i++) {
		for (int k = 0; k <= 4; k++) {
			if (answhite[x1 + i + k][y1 + i + k] == 0 && ansblack[x1 + i + k][y1 + i + k] == 0)  sym = sym + 2;
			else if (answhite[x1 + i + k][y1 +i + k] == 1 && ansblack[x1 + i + k][y1 + i + k] == 0)
			{
				sym = sym - 14;
	//			countwhite++;
			}
			else if (answhite[x1 + i + k][y1 + i + k] == 0 && ansblack[x1 + i + k][y1 + i + k] == 1)
			{
				sym = sym + 15;
	//			countblack++;
			}
			
		}
		if ((y1 + i + 3 <= prelinen && x1 + i  + 3 <= preroln) && ansblack[x1 + i ][y1 + i ] == 1 && ansblack[x1 + i ][y1 + i + 1] == 1 && ansblack[x1 + i ][y1 + i  + 2] == 1 && ansblack[x1 + i ][y1 + i + 3] == 1) sym = sym + 2000000;
		if ((y1 + i + 3 <= prelinen && x1 + i + 3 <= preroln) && ansblack[x1 + i][y1 + i] == 1 && ansblack[x1 + i + 1][y1 + i + 1] == 1 && ansblack[x1 + i + 2][y1 + i + 2] == 1 && ansblack[x1 + i + 3][y1 + i + 3] == 1) sym = sym + 100000;
		if ((y1 + i + 3 <= prelinen && x1 + i + 3 <= preroln) && answhite[x1 + i][y1 + i] == 1 && answhite[x1 + i + 1][y1 + i + 1] == 1 && answhite[x1 + i + 2][y1 + i + 2] == 1 && answhite[x1 + i + 3][y1 + i + 3] == 1) sym = sym + 16000;
		if ((y1 + i  + 2 <= prelinen && x1 + i  + 2 <= preroln) && ansblack[x1 + i ][y1 + i ] == 1 && ansblack[x1 + i  + 1][y1 + i  + 1] == 1 && ansblack[x1 + i + 2][y1 + i  + 2] == 1) sym = sym + 2000;
		if ((y1 + i  + 2 <= prelinen && x1 + i  + 2 <= preroln) && answhite[x1 + i ][y1 + i ] == 1 && answhite[x1 + i  + 1][y1 + i  + 1] == 1 && answhite[x1 + i  + 2][y1 + i  + 2] == 1) sum = sum + 1600;
		if ((y1 + i  + 1 <= prelinen && x1 + i  + 1 <= preroln) && ansblack[x1 + i ][y1 + i ] == 1 && ansblack[x1 + i  + 1][y1 + i  + 1] == 1) sum = sum + 120;

		if (abs(sym) > sum) sum = abs(sym);
		sym = 0;
	//	if (countblack == 4 && countwhite == 0) return 1000;
	//	else if (countwhite == 4) return 900;
//		else if (countblack == 3 && countwhite == 0) return 100;
//		else if (countwhite == 3 && countblack == 0) return 90;
	//	countwhite = 0; countblack = 0;
	}
	if (fusym == 1) ansblack[x][y] = 0;
	return sum;

}

//左下到右上的权值计算；
int chessweightdowntoup(int x, int y) {

	int x1 = x; int y1 = y; int sum = 0; int sym = 0;

	while (x1 > 0 && y1 <prelinen ) {
		x1--; y1++;
	}

	//循环停止时x1，y1至少有一个为0；
	while (x1 <= preroln && y1 >=0) {
		if (answhite[x1][y1] == 0 && ansblack[x1][y1] == 0) {
			sym = sym + 2;
		}
		x1++; y1--;
	}

	if (sym > sum) sum = sym;
	sym = 0;
	//无法围成五子的特例
	x1 = x; y1 = y;
	int countminorx = 0;
	while (x1 > 0 && y1 < prelinen) {
		countminorx++;
		x1--; y1++;
	}
	while (x1 <= preroln && y1 >= 0) {
		sum = sum + 2;
		x1++; y1--;
	}
	x1 = x; y1 = y;
	if (countminorx > 2) countminorx = 2;
	int countplusy = 0;
	while (x1 < preroln && y1>0) {
		countplusy++;
		x1++; y--;
	}
	x1 = x; y1 = y;
	if (countplusy > 2) countplusy = 2;
	int count0 = 0;

	for (int i = -countminorx; i <= 0; i++) {
		if (answhite[x1 + i][y1 - i] ==1) {
			count0++; break;
		}
	}
	for (int i = 0; i <= countplusy; i++) {
		if (answhite[x1 + i][y1 - i] == 1) {
			count0++; break;
		}
	}
	if (count0 == 2) return 0;
	//特例结束；

	x1 = x; y1 = y;
	int countminorxr = 0;
	while (x1 > 0 && y1 < prelinen) {
		countminorxr++;
		x1--; y1++;
	}
	x1 = x; y1 = y;
	if (countminorxr > 4) countminorxr = 4;
	int countplusyr = 0;
	while (x1 < preroln && y1>0) {
		countplusyr++;
		x1++; y--;
	}
	x1 = x; y1 = y;
	if (countplusyr > 4) countplusyr = 4;
	 count0 = 0;
	//常规计算
	sym = 0;// int countwhite = 0; int countblack = 0;
	int ansym = 0;
	if (ansblack[x][y] == 0 && answhite[x][y] == 0) {
		ansblack[x][y] = 1;
		ansym = 1;
	}
	for (int i = -countminorxr; i + 4 <= countplusyr; i++) {
		int maxsym = 0; int max=0;
		for (int k = 0; k <= 4; k++) {

			if (answhite[x1 + i + k][y1 - i - k] == 0 && ansblack[x1 + i + k][y1 - i - k] == 0)  maxsym = maxsym + 2;
		}
		if (maxsym > max) max = maxsym;
		for (int k = 0; k <= 4; k++) {
			if (answhite[x1 + i + k][y1 - i - k] == 0 && ansblack[x1 + i + k][y1 - i - k] == 0)  sym = sym + 2;
			else if (answhite[x1 + i + k][y1 - i - k] == 1 && ansblack[x1 + i + k][y1 - i - k] == 0)
			{
				sym = sym - 14;
			}
			else if (answhite[x1 + i + k][y1 - i - k] == 0 && ansblack[x1 + i + k][y1 - i - k] == 1)
			{
				sym = sym + 15;
				//	countblack++;
			}
		}
		for (int k = 0; k <= 4; k++) {

			if (answhite[x1 + i + k][y1 - i - k] == 0 && ansblack[x1 + i + k][y1 - i - k] == 0)  sym = sym + 2;
			else if (answhite[x1 + i + k][y1 - i - k] == 1 && ansblack[x1 + i + k][y1 - i - k] == 0)
			{
				sym = sym - 14;
				//	countwhite++;
			}
			else if (answhite[x1 + i + k][y1 - i - k] == 0 && ansblack[x1 + i + k][y1 - i - k] == 1)
			{
				sym = sym + 15;
				//	countblack++;
			}
		}
		if ((y1 - i - 4 >= 0 && x1 + i  + 4 <= preroln) && ansblack[x1 + i ][y1 - i ] == 1 && ansblack[x1 + i+1 ][y1 - i  - 1] == 1 && ansblack[x1 + i+2 ][y1 - i  - 2] == 1 && ansblack[x1 + i+3 ][y1 - i  - 3] == 1&&ansblack[x1+i+4][y1-i-4]==1) sum = sum + 2000000;
		if ((y1 - i - 3 >= 0 && x1 + i + 3 <= preroln) && ansblack[x1 + i][y1 - i] == 1 && ansblack[x1 + i + 1][y1 - i - 1] == 1 && ansblack[x1 + i + 2][y1 - i - 2] == 1 && ansblack[x1 + i + 3][y1 - i - 3] == 1) sum = sum + 20000;
		if ((y1 - i  - 2 >= 0 && x1 + i + 2 <= preroln) && ansblack[x1 + i ][y1 - i ] == 1 && ansblack[x1 + i  + 1][y1 - i  - 1] == 1 && ansblack[x1 + i + 2][y1 - i  - 2] == 1) sum = sum + 5000;
		if ((y1 - i - 3 >= 0 && x1 + i + 3 <= preroln) && answhite[x1 + i][y1 - i] == 1 && answhite[x1 + i + 1][y1 - i - 1] == 1 && answhite[x1 + i + 2][y1 - i - 2] == 1 && answhite[x1 + i + 3][y1 - i - 3] == 1) sum = sum + 8000;
		if ((y1 - i  - 2 >= 0 && x1 + i  + 2 <= preroln) && answhite[x1 + i ][y1 - i ] == 1 && answhite[x1 + i  + 1][y1 - i - 1] == 1 && answhite[x1 + i  + 2][y1 - i  - 2] == 1) sum = sum + 1600;
		if ((y1 - i  - 1 >= 0 && x1 + i  + 1 <= preroln) && ansblack[x1 + i ][y1 - i ] == 1 && ansblack[x1 + i  + 1][y1 - i  - 1] == 1) sum = sum + 120;
		if (abs(sym) > sum) sum = abs(sym);
		sym = 0;
	}
	if (ansym == 1) ansblack[x][y] = 0;

	return sum;





}
	









