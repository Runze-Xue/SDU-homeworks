int chessweightline(int x,int y)
{	
	int x1 = x; int y1 = y; int sum = 0; int sym = 0;
	//int keepx = 0; int keepy = 0;
	//一行当中求权值
	

	int left = 0;
	if (x1 - 4 >= 0) left = x1 - 4;
	else if (x1 - 3 >= 0) left = x1 - 3;
	else if (x1 - 2 >= 0) left = x1 - 2;
	else if (x1 - 1 >= 0) left = x1 - 1;
	else if (x1 == 0) left = 0;
	int right = 0;
	if (x1 + 4 <=preroln ) right = x1 + 4;
	else if (x1 + 3 <=preroln ) right = x1 + 3;
	else if (x1 + 2 <=preroln ) right = x1 + 2;
	else if (x1 +1  <=roln) right = x1 + 1;
	else if (x1 == preroln) right = preroln;
	int count0 = 0;  //count0用来计算返回值0的白子数目，如果等于2则证明返回0
	//特例：左右端left right 都不够大,则在范围内存在一个白子，则返回权值0
	if (x1 - left <= 2 &&right-x1<=2) {
		for (int i = left; i <= right; i++) {
			if (answhite[i][y1] == 1) return 0;
		}
	}
	
	if (x1 - left <= 2 && right - x1 > 2) {
		for (int i = left; i < x1 ; i++) {
			if (answhite[i][y1] == 1) {
				count0++;
				if (right == preroln && right - i <= 4) return 0;

				break;
			}
		}
		for (int i = x1; i <= x1 + 2; i++) {
			if (answhite[i][y1] == 1) {
				count0++;
				if (left == 0 && i - left <= 4) return 0;
				break;
			}
		}
		if (count0 == 2) return 0;
	}
	count0 = 0;
	if (x1 - left > 2 && right - x1 <= 2) {
		for (int i = x1 - 2; i < x1; i++) {
			if (answhite[i][y1] == 1) {
				if (right == preroln && right - i <= 4)
					return 0;

				count0++; break;
			}
		}
		for (int j = x1; j <= right; j++) {
			if (answhite[j][y1] == 1) {
				if (left == 0 && j - left <= 4)
					return 0;
				count0++; break;
			}
		}
		if (count0 == 2) return 0;
	}
	count0 = 0;
	if (x1 - left > 2 && right - x1 > 2) {
		for (int i = x1 - 2; i < x1 ; i++) {
			if (answhite[i][y1] == 1) {

				count0++; break;
			}
		}
		for (int i = x1; i <= x1 + 2; i++) {
			if (answhite[i][y1] == 1) {
				count0++; break;
			}
		}
		if (count0 == 2) return 0;
	}
	//特例结束
	//全局寻找空位置落子

	for (int i = 0; i <= preroln; i++) {
		if (answhite[i][y1] == 0 && ansblack[i][y1] == 0)
			sym=sym+2;
	}
	if (sym > sum) sum = sym;
	sym = 0;
	//全局寻找空位置落子结束
	
	//局部寻优落子开始
	int countwhite = 0;
	int countblack = 0;

	for (int i = left; i +4<= right; i++) {
		for (int k = 0; k <= 4; k++) {
			if (ansblack[i + k][y1] == 0 && answhite[i + k][y1] == 0)
			{
				sym = sym + 2;

			}
			else if (ansblack[i + k][y1] == 1 && answhite[i + k][y1] == 0)
			{
				sym = sym + 15;
				countblack++;
			}
			else if (ansblack[i + k][y1] == 0 && answhite[i + k][y1] == 1)
			{
				sym = sym - 14;
				countwhite++;
			}
		}
			if (abs(sym) > sum) sum = abs(sym);
			sym = 0;
			if (countblack == 4 && countwhite == 0) return 1000000;
			else if (countwhite == 4) return 90000;
			else if (countblack == 3 && countwhite == 0) return 10000;
			else if (countwhite == 3 && countblack == 0) return 9000;
			countwhite = 0; countblack = 0;
		
		
	}
	printf("1ok");
	return sum;

}