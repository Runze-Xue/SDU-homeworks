#include<iostream>
int Fibo(int n) {
	if (n == 0) return 1;
	if (n == 1) return 1;
	return Fibo(n - 1) + Fibo(n - 2);
}
int main() {
	int n = 0;
	std::cin >> n;
	std::cout<<Fibo(n);
	return 0;
}