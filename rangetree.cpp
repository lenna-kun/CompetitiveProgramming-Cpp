#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
#define i64 int64_t
#define endl "\n"
#define all(a) a.begin(),a.end()
#define rep(n) for(int i = 0; i < (n); i++)
#define rrep(i,a,b) for(int i = (b) - 1; i >= (a); i--)

template <class S, S (*op)(S, S), S (*e)()> class RangeTree {
private:
  int n;
  using pt = pair<i64, i64>;
  vector<pt> pts;
  vector<vector<i64>> d;
  vector<segtree<S, op, e>> segtrees;
  void set(int v, i64 y, S val) {
    segtrees[v].set(lower_bound(all(d[v]), y) - d[v].begin(), val);
  }
  void add(int v, i64 y, S val) {
    int i = lower_bound(all(d[v]), y) - d[v].begin();
    segtrees[v].set(i, op(segtrees[v].get(i), val));
  }
  S prod(int v, i64 yl, i64 yr) {
    return segtrees[v].prod(
      lower_bound(all(d[v]), yl) - d[v].begin(),
      lower_bound(all(d[v]), yr) - d[v].begin()
    );
  }

public:
  void add_point(i64 x, i64 y) { pts.emplace_back(x, y); }
  void build() {
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());
    n = pts.size();
    d.resize(2*n);
    rep (n) d[n+i] = {pts[i].second};
    rrep (i, 1, n) {
      auto &lch = d[i*2];
      auto &rch = d[i*2+1];
      merge(all(lch), all(rch), back_inserter(d[i]));
      d[i].erase(unique(all(d[i])), d[i].end());
    }
    for (auto &v: d) segtrees.emplace_back(v.size());
  }
  // 座標x,yに値valを置く
  void set(i64 x, i64 y, S val) {
    int i = lower_bound(all(pts), pt{x, y}) - pts.begin();
    assert(i < n and pts[i] == make_pair(x, y));
    for (i+=n; i; i>>=1) set(i, y, val);
  }
  // 座標x,yに値valを重ねて置く
  void add(i64 x, i64 y, S val) {
    int i = lower_bound(all(pts), pt{x, y}) - pts.begin();
    assert(i < n and pts[i] == make_pair(x, y));
    for (i+=n; i; i>>=1) add(i, y, val);
  }
  // [xl, xr), [yl, yr)
  S prod(i64 xl, i64 yl, i64 xr, i64 yr) {
    auto cmp = [](const pt &l, const pt &r) { return l.first < r.first; };
    int l = n + (lower_bound(all(pts), pt{xl, 0}, cmp) - pts.begin());
    int r = n + (lower_bound(all(pts), pt{xr, 0}, cmp) - pts.begin());
    S ret = e();
    while (l < r) {
      if (l & 1) ret = op(ret, prod(l++, yl, yr));
      if (r & 1) ret = op(ret, prod(--r, yl, yr));
      l >>= 1, r >>= 1;
    }
    return ret;
  }
  S get(i64 x, i64 y) { return prod(x, y, x+1, y+1); }
};

// verify: https://judge.yosupo.jp/problem/point_add_rectangle_sum
i64 e() { return 0; }
i64 op(i64 a, i64 b) { return a + b; }

void Main() {
  int n, q;
  cin >> n >> q;
  vector<vector<i64>> query;
  RangeTree<i64, op, e> rt2d;
  rep (n) {
    i64 x, y, w;
    cin >> x >> y >> w;
    rt2d.add_point(x, y);
    query.push_back({ 0, x, y, w });
  }
  rep (q) {
    int t;
    cin >> t;
    if (t == 0) {
      i64 x, y, w;
      cin >> x >> y >> w;
      rt2d.add_point(x, y);
      query.push_back({ 0, x, y, w });
    } else {
      i64 l, d, r, u;
      cin >> l >> d >> r >> u;
      query.push_back({ 1, l, d, r, u });
    }
  }
  rt2d.build();
  // ここまで前処理

  for (vector<i64> &q: query) {
    if (q[0])
      cout << rt2d.prod(q[1], q[2], q[3], q[4]) << endl;
    else
      rt2d.add(q[1], q[2], q[3]);
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout << setprecision(10) << fixed;
  Main();
}