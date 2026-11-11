#pragma GCC optimize(3,"Ofast","inline")
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define int long long

using VT = int;

const int dir[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} }, n = 3;
int beg, des, mat[3][3], cntr = 0, dis[9][2];
map<int, int> pre;
struct Node {
	int sta, stp;
	VT g, h;
	
	bool operator()(const Node& a, const Node& b) {
		return a.g + a.h < b.g + b.h;
	}
	inline VT fg(int x) {
		return -x;
	}
	inline VT fh(int x) {
		int ans = 0;
		for(int i=0; i<n; ++i){
			for(int j=0; j<n; ++j){
				int t = x % 10; x /= 10;
				ans += abs(i-dis[t][0]) + abs(j-dis[t][1]);
			}
		}
		return ans;
	}
	Node(int sta = 0, int stp = 0) :sta(sta), stp(stp) {
		g = fg(stp); h = fh(sta);
	}
};
Node makenxt(Node& lst, int id) {
	int cur = lst.sta, x = 0, y = 0;
	for (int i = n - 1; i >= 0; --i) {
		for (int j = n - 1; j >= 0; --j) {
			mat[i][j] = cur % 10;
			cur /= 10;
			if (mat[i][j] == 0) x = i, y = j;
		}
	}
	int nx = x + dir[id][0], ny = y + dir[id][1];
	if (nx >= 0 && ny >= 0 && nx < n && ny < n) {
		swap(mat[x][y], mat[nx][ny]);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				cur = cur * 10 + mat[i][j];
			}
		}
		if (pre[cur] == 0) {
			cntr ++;
			return Node(cur, lst.stp + 1);
		}
	}
	return Node(-1);
}
Node solve() {
	int t = des;
	for(int i=n-1; i>=0; --i){
		for(int j=n-1; j>=0; --j){
			dis[t%10][0] = i, dis[t%10][1] = j;
			t /= 10;
		}
	}
	priority_queue<Node, vector<Node>, Node> Q;
	pre[beg] = -1;
	if (beg == des) return Node(beg, 1);
	Q.push(Node(beg, 1));
	while (!Q.empty()) {
		Node x = Q.top(); Q.pop();
		for (int j = 0; j < 4; ++j) {
			Node nxt = makenxt(x, j);
			if (nxt.sta != -1) { //no out of boundary or repeat
				pre[nxt.sta] = x.sta;
				if (nxt.sta == des) return nxt;
				Q.push(nxt);
			}
		}
	}
	return Node(-1);
}
void output(){
	stack<int> step;
	int t = des;
	while(t != -1){
		step.push(t); t = pre[t];
	}
	while(!step.empty()){
		t = step.top(); step.pop();
		for(int i=n-1; i>=0; --i){
			for(int j=n-1; j>=0; --j){
				mat[i][j] = t % 10; t /= 10;
			}
		}
		for(int i=0; i<n; ++i){
			for(int j=0; j<n; ++j){
				cout << mat[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
}
signed main() {
	cin >> beg >> des;
	DWORD time_start, time_end;
    time_start = GetTickCount();
    
	Node nd = solve();
	cout << nd.stp-1 << " " << cntr << "\n";
	time_end = GetTickCount();
    cout << "Time = " << (time_end - time_start) << "ms\n ";
	system("pause");
	output();
}
