#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
#define i64 int64_t
#define f64 double
const i64 inf = 1e17;
const int dx[] = {0, 1, 0, -1, 1, 1, -1, -1};
const int dy[] = {1, 0, -1, 0, 1, -1, 1, -1};
#define endl "\n"
#define all(a) a.begin(),a.end()
#define overload(_1,_2,_3,_4,name,...) name
#define _rep1(n) for(int i = 0; i < (n); i++)
#define _rep2(i,n) for(int i = 0; i < (n); i++)
#define _rep3(i,a,b) for(int i = (a); i < (b); i++)
#define _rep4(i,a,b,c) for(int i = (a); i < (b); i += (c))
#define rep(...) overload(__VA_ARGS__,_rep4,_rep3,_rep2,_rep1)(__VA_ARGS__)
#define _rrep1(n) for(int i = (n) - 1; i >= 0; i--)
#define _rrep2(i,n) for(int i = (n) - 1; i >= 0; i--)
#define _rrep3(i,a,b) for(int i = (b) - 1; i >= (a); i--)
#define _rrep4(i,a,b,c) for(int i = (b) - 1; i >= (a); i -= (c))
#define rrep(...) overload(__VA_ARGS__,_rrep4,_rrep3,_rrep2,_rrep1)(__VA_ARGS__)
#define ForEach(a,b) for_each(a.begin(),a.end(),b)
#define NextAfter(x) x = nextafter(x, INFINITY)
template <class T> bool chmin(T& a, T b){ if(a > b){ a = b; return 1; } return 0; }
template <class T> bool chmax(T& a, T b){ if(a < b){ a = b; return 1; } return 0; }
struct Edge { int to; i64 cost; Edge(int to, i64 cost) : to(to), cost(cost) {} };
using Graph = vector<vector<Edge>>;

// O(E log V)
pair<vector<i64>, vector<int>> dijkstra(int s, i64 n, Graph &g) {
  vector<int> prev(n, -1);
  vector<i64> d(n, inf);
  d[s] = 0;
  priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> que;
  que.push({0, s});
  while (!que.empty()) {
    pair<i64, i64> p = que.top();que.pop();
    int v = p.second;
    if (d[v] < p.first) continue;
    for (Edge &e: g[v]) {
      if (d[e.to] > d[v] + e.cost) {
        d[e.to] = d[v] + e.cost;
        prev[e.to] = v;
        que.push({d[e.to], e.to});
      }
    }
  }
  return {d, prev};
}

// 経路復元
vector<pair<int, int>> get_path(int t, vector<int> &prev) {
  vector<pair<int, int>> path;
  for (; prev[t] != -1; t = prev[t]) path.push_back({prev[t], t});
  reverse(all(path));
  return move(path);
}

// # O(EV)
// def belman_ford(s, n, g): # (始点, ノード数, グラフ情報) g: [[(to, cost), ...], ...]
//   d = [INF] * n
//   d[s] = 0
//   update = 1
//   for _ in range(n):
//     update = 0
//     for v, e in enumerate(g):
//       for to, cost in e:
//         if d[v] != INF and d[v] + cost < d[to]:
//           d[to] = d[v] + cost
//           update = 1
//     if not update:
//       break
//   else:
//     return None
//   return d

// # O(V^3)
// def warshall_floyd(n, cost): # cost[i][j]: 頂点iから頂点jへ到達するための辺の重み(ただし，辺が存在しない場合はINF，cost[i][i]=0)
//   for k in range(n):
//     for i in range(n):
//       for j in range(n):
//         if cost[i][k] != INF and cost[k][j] != INF:
//           cost[i][j] = min(cost[i][j], cost[i][k] + cost[k][j])
//   # cost[i][j]: 頂点iから頂点jへ到達するための辺コストの和
//   for i in range(n):
//     if cost[i][i] < 0:
//       return False
//   return True