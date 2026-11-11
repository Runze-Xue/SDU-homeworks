#include<iostream>
int m;
int f(int x) {
	if (x > 0) return 1;
	else if (x == 0) return 0;
	else return -1;
}
int sum=0; int count = 0;
int main() {
	int n = 0;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> m;
		if (f(m)==1) {
			sum+=m; count++;
		}
	}
	if (count == 0) std::cout << "0";
	else {
		double ans1 = 0;
		ans1 = 1.0 * sum / count;
		std::cout << ans1;
	}
	return 0;
}