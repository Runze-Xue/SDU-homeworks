#include<iostream>
double  p( double n,  double x) {
	if (n == 0) return 0;
	if (n == 1) return x;
	return ((2 * n - 1) * x * p(n - 1, x) - (n - 1) * p(n - 2, x)) / n;
}
int main() {
	double  n, x;
	std::cin >> n >> x;
	std::cout<<p(n, x);
	return 0;
}