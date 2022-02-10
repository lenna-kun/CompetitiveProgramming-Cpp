#include <bits/stdc++.h>
using namespace std;
#define all(a) a.begin(),a.end()
#define rep(n) for(int i = 0; i < (n); i++)

struct LowLink {
  const vector<vector<int>> &g;
  vector<int> used, ord, low;
  vector<int> aps;  // articulation points
  vector<pair<int, int>> bridges;

  LowLink(const vector<vector<int>> &g_) : g(g_) {
    used.assign(g.size(), 0);
    ord.assign(g.size(), 0);
    low.assign(g.size(), 0);
    int order = 0;
    rep (g.size()) if (!used[i]) order = dfs(i, order, -1);
    sort(all(aps)); // 必要ならソートする
    sort(all(bridges)); // 必要ならソートする
  }

  int dfs(int node, int order, int par) { // node:探索中の頂点, order:dfsで何番目に探索するか, par:idの親
    used[node] = true;
    ord[node] = order++;
    low[node] = ord[node];
    bool is_aps = false;
    int count = 0; // 子の数
    for (auto &e : g[node]) {
      if (!used[e]) {
        count++;
        order = dfs(e, order, node);
        low[node] = min(low[node], low[e]);
        if (par != -1 && ord[node] <= low[e]) is_aps = true; 
        if (ord[node] < low[e]) bridges.emplace_back(min(node, e), max(node, e)); // 条件を満たすので橋  
      } else if (e != par) { // eが後退辺の時
        low[node] = min(low[node], ord[e]);
      }
    }
    if (par == -1 && count >= 2) is_aps = true; 
    if (is_aps) aps.push_back(node);
    return order;
  }
};