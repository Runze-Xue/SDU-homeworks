#include <bits/stdc++.h>
using namespace std;

#define pb push_back

typedef pair<int, int> pii;
typedef long long ll;

const int N = 2e5 + 5;

int n;

void solve() {
    cin >> n;
    vector<pii> e[n + 1];
    vector<double> f(n + 1);
    vector<int> depth(n + 1);
    for (int i = 1; i <= n; i++)
        cout << f[i] << " \n"[i == n];
    depth[0] = 0;
    for (int i = 1; i < n; i++) {
        int x, y, k;
        cin >> x >> y >> k;
        e[x].pb(make_pair(y, k));
        e[y].pb(make_pair(x, k));
    }
//     auto dfs = [&](int x, int fa) {
//         depth[x] = depth[fa] + 1;
//         for (auto ver : e[x]) {

//         }
//     };
//     dfs(1, 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int tc; cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}