#include<bits/stdc++.h>

using namespace std;

const int maxn = 200005;
struct Edge{
    int u, v, w, nxt;
}edge[maxn];

int cnt = 0;
int head[maxn];
void addedge(int u, int v, int w){
    cnt++;
    edge[cnt].u = u;
    edge[cnt].v = v;
    edge[cnt].w = w;
    edge[cnt].nxt = head[u];
    head[u] = cnt;
}

int onenum[maxn];
int zeronum[maxn];
int tor[maxn];
int depth[maxn];
int check[maxn];
void pre(int loc, int dep, int zero, int one){ // 
    if(check[loc] == 1) return;
    check[loc] = 1;
    depth[loc] = dep;
    onenum[loc] = one;
    zeronum[loc] = zero;
    for(int i = head[loc]; i != 0; i = edge[i].nxt){
        int v = edge[i].v;
        int w = edge[i].w;
        if(check[v] == 0){
            // std::cout<<" loc "<<loc<<" v:: "<<v<<"  w  "<< w <<'\n';
            if(w == 0){
                // std::cout<<" v "<<v<<" zero  "<<zeronum[loc]+1<<"  one  "<<onenum[loc]<<'\n';
                pre(v, dep + 1, zeronum[loc] + 1, onenum[loc]);
            }
            else {
                pre(v, dep + 1, zeronum[loc], onenum[loc] + 1);
            }
        }
    }
}
void dfs(int loc){
    if(tor[loc] == 1){// 叶子
        return ;
    }
    if(check[loc] == 1) return;
    check[loc] = 1;
    for(int i = head[loc]; i != 0; i = edge[i].nxt){
        int v = edge[i].v;
        if(check[v] == 0) {
            dfs(v);
        }
    }
    if(depth[loc] % 2 == 1){ // Grammy 选 0 占比小的
        // std::cout<<"in grammy" <<'\n';
        for(int i = head[loc]; i != 0; i = edge[i].nxt){
            int v =  edge[i].v;
            if(check[v] == 0){
            // std::cout<<"  loc  "<<loc<<"  v  "<<v<<'\n';
                if(onenum[loc] + zeronum[loc] == 0){
                    zeronum[loc] = zeronum[v]; onenum[loc] = onenum[v];
                }
                double cnt = 1.0 * zeronum[loc] / (1.0 * (zeronum[loc] + onenum[loc]));
                double soncnt = 1.0 * zeronum[v] / (1.0 * (zeronum[v] + onenum[v]));
                // std::cout<<"cnt  "<<cnt<<"  soncnt  "<<soncnt<<'\n';
                // std::cout<<"zeronum  "<<zeronum[loc]<<"  onenum  "<<onenum[loc]<<'\n';
                if(cnt > soncnt){
                    zeronum[loc] = zeronum[v]; onenum[loc] = onenum[v];
                }
            }
        }
    }
    else{ // Oscar 选0 占比大的
        for(int i = head[loc]; i != 0; i = edge[i].nxt){
            int v =  edge[i].v;
            if(check[v] == 0){
                if(zeronum[loc] + onenum[loc] == 0){
                    zeronum[loc] = zeronum[v]; onenum[loc] = onenum[v];
                }
                double cnt = 1.0 * zeronum[loc] / (1.0 * (zeronum[loc] + onenum[loc]));
                double soncnt = 1.0 * zeronum[v] / (1.0 * (zeronum[v] + onenum[v])); 
                if(cnt < soncnt){
                    zeronum[loc] = zeronum[v]; onenum[loc] = onenum[v];
                }
            }
        }
    }
    check[loc] = 0; // 解锁
}
void solve(){
    int n; std::cin >> n;
    memset(tor, 0, (n + 1)*sizeof(int));
    memset(depth, 0, (n + 1)*sizeof(int));
    memset(onenum, 0, (n + 1)*sizeof(int));
    memset(zeronum, 0, (n + 1)*sizeof(int));
    memset(head, 0, (n + 1) * sizeof(int));
    cnt = 0;
    for(int i = 1; i < n; i++){
        int x, y, k; std::cin>> x >> y >> k;
        addedge(x, y, k);
        addedge(y, x, k);
        tor[x]++; tor[y]++;
    }
    memset(check, 0, (n + 1)*sizeof(int));
    pre(1, 1, 0, 0);
    memset(check, 0, (n + 1)*sizeof(int));
    dfs(1);
    printf("%.12lf\n", (1.0 * onenum[1]) / (1.0 * onenum[1] + 1.0 * zeronum[1]));
}

int main(){
    int t; std::cin >> t;
    while(t--){
        solve();
    }
}