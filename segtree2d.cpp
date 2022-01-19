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
  void add_point(i64 x, i64 y) {
    pts[x].push_back(y);
    pt.push_back(x);
  }
  void init() {
    sort(all(pt));
    pt.erase(unique(all(pt)), pt.end());

    int n = pt.size();
    size = 1; while (size < n) size *= 2;
    idx.resize(2*size);

    int i = 0;
    for (auto &[x, ys]: pts) idx[(i++)+size] = ys;
    // pts.clear();
    rrep (i, 1, 2*size) {
      sort(all(idx[i]));
      idx[i].erase(unique(all(idx[i])), idx[i].end());
      for (i64 &j: idx[i]) idx[i / 2].push_back(j);
    }
    st.resize(2*size);
    rep (i, 1, 2*size) st[i] = segtree<S, op, e>(idx[i].size());
  }
  void set(i64 x, i64 y, S v) { update(x, y, v, true); }
  void add(i64 x, i64 y, S v) { update(x, y, v, false); }
  S prod(i64 sx, i64 sy, i64 tx, i64 ty) {
    return prod(sx, tx, sy, ty, 1, 0, size);
  }
private:
  int size;
  map<i64, vector<i64>> pts;
  vector<i64> pt;
  vector<segtree<S, op, e>> st;
  vector<vector<i64>> idx;
  void update(i64 x, i64 y, S v, bool mode) {
    int xx = lower_bound(all(pt), x) - pt.begin();
    assert(xx < size);
    xx += size;
    while (xx) {
      int yy = lower_bound(all(idx[xx]), y) - idx[xx].begin();
      assert(yy != idx[xx].size());
      assert(y == idx[xx][yy]);
      if (mode) st[xx].set(yy, v);
      else st[xx].set(yy, st[xx].get(yy) + v);
      xx >>= 1;
    }
  }
  S prod(i64 sx, i64 tx, i64 sy, i64 ty, int k, int l, int r) {
    assert(k < size * 2);
    assert(l < r);
    int sxx = lower_bound(all(pt), sx) - pt.begin();
    int txx = lower_bound(all(pt), tx) - pt.begin();
    if (r <= sxx or txx <= l) return e();
    if (sxx <= l and r <= txx) {
      int syy = lower_bound(all(idx[k]), sy) - idx[k].begin();
      int tyy = lower_bound(all(idx[k]), ty) - idx[k].begin();
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
  vector<vector<i64>> query;
  SegTree2d<i64, op, e> st2d;
  rep (n) {
    i64 x, y, w;
    cin >> x >> y >> w;
    st2d.add_point(x, y);
    query.push_back({ 0, x, y, w });
  }
  rep (q) {
    int t;
    cin >> t;
    if (t == 0) {
      i64 x, y, w;
      cin >> x >> y >> w;
      st2d.add_point(x, y);
      query.push_back({ 0, x, y, w });
    } else {
      i64 l, d, r, u;
      cin >> l >> d >> r >> u;
      query.push_back({ 1, l, d, r, u });
    }
  }
  st2d.init();
  // ここまで前処理

  for (vector<i64> &q: query) {
    if (q[0])
      cout << st2d.prod(q[1], q[2], q[3], q[4]) << endl;
    else
      st2d.add(q[1], q[2], q[3]);
  }
}