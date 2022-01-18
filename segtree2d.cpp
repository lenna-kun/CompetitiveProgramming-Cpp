#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
#define i64 int64_t
#define endl "\n"
#define all(a) a.begin(),a.end()
#define overload(_1,_2,_3,_4,name,...) name
#define _rep1(n) for(int i = 0; i < (n); i++)
#define _rep2(i,n) for(int i = 0; i < (n); i++)
#define _rep3(i,a,b) for(int i = (a); i < (b); i++)
#define _rep4(i,a,b,c) for(int i = (a); i < (b); i += (c))
#define rep(...) overload(__VA_ARGS__,_rep4,_rep3,_rep2,_rep1)(__VA_ARGS__)
#define rrep(i,a,b) for(int i = (b) - 1; i >= (a); i--)

i64 e() { return 0; }
i64 op(i64 a, i64 b) { return a + b; }

template <class S, S (*op)(S, S), S (*e)()> class SegTree2d {
public:
  void init(vector<vector<int>> &v) {
    int n = v.size();
    size = 1; while (size < n) size *= 2;
    index.resize(2 * size);
    rep (n) for (int &j: v[i]) index[i + size].push_back(j);
    rrep (i, 1, size * 2) {
      sort(all(index[i]));
      index[i].erase(unique(all(index[i])), index[i].end());
      for (int &j: index[i]) index[i / 2].push_back(j);
    }
    st.resize(2 * size);
    rep (i, 1, size * 2) st[i] = segtree<S, op, e>(index[i].size());
  }
  void set(int x, int y, S v) { update(x, y, v, true); }
  void add(int x, int y, S v) { update(x, y, v, false); }
  S prod(int sx, int sy, int tx, int ty) {
    return prod(sx, tx, sy, ty, 1, 0, size);
  }
private:
  int size;
  vector<segtree<S, op, e>> st;
  vector<vector<int>> index;
  void update(int x, int y, S v, bool mode) {
    assert(x < size);
    x += size;
    while (x) {
      int yy = lower_bound(all(index[x]), y) - index[x].begin();
      assert(yy != index[x].size());
      assert(y == index[x][yy]);
      if (mode) st[x].set(yy, v);
      else st[x].set(yy, st[x].get(yy) + v);
      x >>= 1;
    }
  }
  S prod(int sx, int tx, int sy, int ty, int k, int l, int r) {
    assert(k < size * 2);
    assert(l < r);
    if (r <= sx or tx <= l) return e();
    if (sx <= l and r <= tx) {
      int syy = lower_bound(all(index[k]), sy) - index[k].begin();
      int tyy = lower_bound(all(index[k]), ty) - index[k].begin();
      return st[k].prod(syy, tyy);
    }
    int md = (l + r) / 2;
    S le = prod(sx, tx, sy, ty, k * 2, l, md);
    S ri = prod(sx, tx, sy, ty, k * 2 + 1, md, r);
    return op(le, ri);
  }
};

// verify: https://judge.yosupo.jp/problem/point_add_rectangle_sum
int main() {
  int n, q;
  cin >> n >> q;
  pair<map<i64, int>, map<i64, int>> comp;
  pair<vector<i64>, vector<i64>> decomp;
  vector<vector<i64>> query;
  rep (n) {
    vector<i64> tmp(4); tmp[0] = 0;
    cin >> tmp[1] >> tmp[2] >> tmp[3];
    decomp.first.push_back(tmp[1]);
    decomp.second.push_back(tmp[2]);
    query.push_back(tmp);
  }
  rep (q) {
    int t;
    cin >> t;
    if (t == 0) {
      i64 x, y, w;
      cin >> x >> y >> w;
      decomp.first.push_back(x);
      decomp.second.push_back(y);
      query.push_back({ 0, x, y, w });
    } else {
      i64 l, d, r, u;
      cin >> l >> d >> r >> u;
      decomp.first.push_back(l);
      decomp.first.push_back(r);
      decomp.second.push_back(d);
      decomp.second.push_back(u);
      query.push_back({ 1, l, d, r, u });
    }
  }
  // 座標圧縮
  sort(all(decomp.first)); sort(all(decomp.second));
  decomp.first.erase(unique(all(decomp.first)), decomp.first.end());
  decomp.second.erase(unique(all(decomp.second)), decomp.second.end());
  rep (decomp.first.size()) comp.first[decomp.first[i]] = i;
  rep (decomp.second.size()) comp.second[decomp.second[i]] = i;

  vector<vector<int>> idx(decomp.first.size());
  for (vector<i64> &q: query) {
    if (q[0]) continue;
    idx[comp.first[q[1]]].push_back(comp.second[q[2]]);
  }
  // ここまで前処理

  SegTree2d<i64, op, e> st2d;
  st2d.init(idx);
  for (vector<i64> &q: query) {
    if (q[0] == 0)
      st2d.add(comp.first[q[1]], comp.second[q[2]], q[3]);
    else
      cout << st2d.prod(
        comp.first[q[1]], comp.second[q[2]], comp.first[q[3]], comp.second[q[4]]
      ) << endl;
  }
}