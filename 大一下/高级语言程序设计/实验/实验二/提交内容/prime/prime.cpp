#include<iostream>
#include<math.h>
using namespace std;
int judge(int math) {
	if (math % 2 == 0) return 0;
	if (math % 3 == 0) return 0;
	
	for (int i = 5; i <= sqrt(math); i += 2) {
		if (math % i == 0) return 0;
	}
	return 1;
}
double sum;
int main() {
	int m, n;
	cin >> m >> n;
	for (int i = m; i <= n; i++) {
		if (i == 1) continue;
		else if (i == 2) {
			sum += sqrt(2);
		}
		else if (i == 3) {
			sum += sqrt(3);
		}
		else {
			if (judge(i)) sum += sqrt(i);
		}
	}
	cout << sum;
	return 0;
}