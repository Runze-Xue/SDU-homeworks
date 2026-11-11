int chessweightuptodown(int x, int y) {
	printf("3start");
	int x1 = x; int y1 = y; int sum = 0; int sym = 0;

	while (x1 > 0 && y1 > 0) {
		x1--; y1--;
	}
	//循环停止时x1，y1至少有一个为0；
	while (x1 <= preroln && y1 <= prelinen) {
		if (answhite[x1][y1] == 0 && ansblack[x1][y1] == 0) {
			sym = sym + 2;
			x1++; y1++;
		}
	}
	if (sym > sum) sum = sym;

	sym = 0;
	x1 = x; y1 = y;
	//通用结束
	if (x1 == y1) {//x1与y1相等，即棋子在斜线上

		int up = 0;
		//通用开始
		if (y1 - 4 >= 0)   up = y1 - 4;
		else if (y1 - 3 >= 0) up = y1 - 3;
		else if (y1 - 2 >= 0) up = y1 - 2;
		else if (y1 - 1 >= 0) up = y1 - 1;
		else if (y1 == 0) up = 0;
		int down = 0;
		if (y1 + 4 <= prelinen) down = y1 + 4;
		else if (y1 + 3 <= prelinen) down = y1 + 3;
		else if (y1 + 2 <= prelinen) down = y1 + 2;
		else if (y1 + 1 <= prelinen) down = y1 + 1;
		else if (y1 == prelinen) down = prelinen;
		int left = 0;
		if (x1 - 4 >= 0) left = x1 - 4;
		else if (x1 - 3 >= 0) left = x1 - 3;
		else if (x1 - 2 >= 0) left = x1 - 2;
		else if (x1 - 1 >= 0) left = x1 - 1;
		else if (x1 == 0) left = 0;
		int right = 0;
		if (x1 + 4 <= preroln) right = x1 + 4;
		else if (x1 + 3 <= preroln) right = x1 + 3;
		else if (x1 + 2 <= preroln) right = x1 + 2;
		else if (x1 + 1 <= preroln) right = x1 + 1;
		else if (x1 == preroln) right = preroln;
		int count0 = 0;


		if (prelinen <= preroln) { //行数比列数少，加和方向上先触碰到y的最大值
			if (y1 - up <= 2 && down - y1 <= 2) {
				for (int i = up; i <= down; i++) {
					if (answhite[i][i] == 1) return 0;
				}
			}//总空间小于5，有一个白子则无法连接为五子，直接返回0
			int count0 = 0;
			if (y1 - up <= 2 && down - y1 > 2) {
				for (int i = up; i <= y1; i++) {
					if (answhite[i][i] == 1) count0++;
					break;
				}
				for (int i = y1; i <= y1 + 2; i++) {
					if (answhite[i][i] == 1) count0++;
					break;
				}
				if (count0 == 2) return 0;
			}
			count0 = 0;
			if (y1 - up > 2 && down - y1 <= 2) {
				for (int i = y1 - 2; i <= y1; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				for (int j = y1; j <= down; j++) {
					if (answhite[j][j] == 1) {
						count0++; break;
					}
				}
				if (count0 == 2) return 0;
			}
			count0 = 0;
			if (y1 - up > 2 && down - y1 > 2) {
				for (int i = y1 - 2; i <= y1; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				for (int i = y1; i <= y1 + 2; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				if (count0 == 2) return 0;
			}
           //特例结束
			//左上到右下，且x1=y1，prelinen<roll的普遍情况开始
				sym = 0;
				int countwhite = 0;
				int countblack = 0;

				for (int i = up; i + 4 <= down; i++) {
					for (int k = 0; k <= 4; k++) {
						if (ansblack[i+k][i + k] == 0 && answhite[i+k][i + k] == 0)
						{
							sym = sym + 2;

						}
						else if (ansblack[i+k][i + k] == 1 && answhite[i+k][i + k] == 0)
						{
							sym = sym + 15;
							countblack++;
						}
						else if (ansblack[i+k][i + k] == 0 && answhite[i+k][i + k] == 1)
						{
							sym = sym - 14;
							countwhite++;
						}
					}
					if (abs(sym) > sum) sum = abs(sym);
					sym = 0;
					if (countblack == 4 && countwhite == 0) return 100000;
					else if (countwhite == 4) return 9000;
					else if (countblack == 3 && countwhite == 0) return 1000;
					else if (countwhite == 3 && countblack == 0) return 900;
					countwhite = 0; countblack = 0;


			}
				return sum;
		}
		else if (prelinen > preroln) {//行数比列数大，加和方向x先触摸到极限
			if (x1 - left <= 2 && right -x1 <= 2) {
				for (int i = left; i <= right; i++) {
					if (answhite[i][i] == 1) return 0;
				}
			}//总空间小于5，有一个白子则无法连接为五子，直接返回0
			int count0 = 0;
			if (x1 - left <= 2 && right - x1 > 2) {
				for (int i = left; i <= x1; i++) {
					if (answhite[i][i] == 1) count0++;
					break;
				}
				for (int i = y1; i <= x1 + 2; i++) {
					if (answhite[i][i] == 1) count0++;
					break;
				}
				if (count0 == 2) return 0;
			}
			count0 = 0;
			if (x1 - left > 2 && right - x1 <= 2) {
				for (int i = x1 - 2; i < x1; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				for (int i = x1; i <= right; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				if (count0 == 2)  return 0;
			}
			count0 = 0;
			if (x1 - left > 2 && right- x1 > 2) {
				for (int i = x1 - 2; i <x1; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				for (int i = x1; i <= x1 + 2; i++) {
					if (answhite[i][i] == 1) {
						count0++; break;
					}
				}
				if (count0 == 2) return 0;
			}
			//特例结束
			 //左上到右下，且x1=y1，prelinen<roll的普遍情况开始
			sym = 0;
			int countwhite = 0;
			int countblack = 0;
				for (int i = left; i + 4 <= right; i++) {
				for (int k = 0; k <= 4; k++) {
						if (ansblack[i + k][i + k] == 0 && answhite[i + k][i + k] == 0)
						{
							sym = sym + 2;

						}
						else if (ansblack[i + k][i + k] == 1 && answhite[i + k][i + k] == 0)
						{
							sym = sym + 15;
							countblack++;
						}
						else if (ansblack[i + k][i + k] == 0 && answhite[i + k][i + k] == 1)
						{
							sym = sym - 14;
							countwhite++;
						}
					}
					if (abs(sym) > sum) sum = abs(sym);
					sym = 0;
					if (countblack == 4 && countwhite == 0) return 100000;
					else if (countwhite == 4) return 9000;
					else if (countblack == 3 && countwhite == 0) return 1000;
					else if (countwhite == 3 && countblack == 0) return 900;
					countwhite = 0; countblack = 0;

				}
				return sum;
		}
	}
	else if (x1 != y1) {//x1比y1大，即棋子在斜线上面，y方面先减到0，加和方面要看preroln和prelinen
		x1 = x; y1 = y;  sum = 0;  sym = 0;
		//一列当中求权值
		//全列求未落子部分的权值开始
		int x2 = x; int y2 = y;
		while (x2 > 0 && y2 > 0) {
			x2--; y2--;
		}
		while (x2 <= preroln && y2 <= prelinen) {
			if (answhite[x2][y2] == 0 && ansblack[x2][y2] == 0) sym = sym + 2;
			x2++; y2++;
		}
		if (sym > sum) sum = sym;
		//全列搜索未落子结束
		sym = 0; x2 = x; y2 = y;
		int countminorr = 0;
		int countplusr = 0;
		while (x2 > 0 && y2 > 0) {
			countminorr++;
			x2--; y2--;
		}
		if (countminorr > 4) countminorr = 4;
		//countminor 是向左上移动的最大步数；
		x2 = x; y2 = y;
		while (x2 < preroln && y2 < prelinen) {
			countplusr++;
			x2++; y2++;
		}
		if (countplusr > 4) countplusr = 4;
		//countplus 是向右下移动的最大步数
		x2 = x; y2 = y;

		int count0 = 0;
		//特例 落点左上，右下有白子，无法形成五子相连
		int countminor = 0;//用来计算向左上方移动的次数
		int countplus = 0;//用来计算向右下方移动的次数
		x1 = x; y1 = y;
		while (x1 > 0 && y1 > 0) {
			countminor++;
			x1--; y1--;
		}
		if (countminor > 2) countminor = 2;//不能超过2
		x1 = x; y1 = y;
		while (x1 < preroln && y1 < prelinen) {
			countplus++;
			x1++; y1++;
		}
		if (countplus > 2) countplus = 2;
		x1 = x; y1 = y;
		//寻找一定不能五子的特例
		 count0 = 0;
		for (int i = 0; i <=countminor ; i++) {
			if (answhite[x1 - i][y1 - i] == 1) {
				count0++; break;
			}
		}
		for (int i = 0; i <= countplus; i++) {
			if (answhite[x1 + i][y1 + i] == 1) {
				count0++; break;

			}
		}
		if (count0 == 2) return 0;
		//特例结束
		//普遍情况开始
		int countblack = 0; int countwhite = 0;
		sym = 0;
		for (int i =-countminorr; i + 4 <=countplusr; i++) {
			for (int k = 0; k <= 4; k++) {
				if (ansblack[x1 + i][y1 + i] == 0 && answhite[x1 + i][y1 + i] == 0)  sym = sym + 2;
				else if (ansblack[x1 + i][y1 + i] == 0 && answhite[x1 + i][y1 + i] == 1)
				{
					sym = sym - 14;
					countwhite++;
				}
				else if (ansblack[x1 + i][y1 + i] == 1 && answhite[x1 + i][y1 + i] == 0)
				{
					sym = sym + 15;
					countblack++;
				}
			}
			if (abs(sym) > sum) sum = abs(sym);
			sym = 0;
			if (countblack == 4 && countwhite == 0) return 100000;
			else if (countwhite == 4) return 9000;
			else if (countblack == 3 && countwhite == 0) return 1000;
			else if (countwhite == 3 && countblack == 0) return 900;
			countwhite = 0; countblack = 0;
		
		}


	}
	return  sum;

	}