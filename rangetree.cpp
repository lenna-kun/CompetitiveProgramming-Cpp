#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t
#define endl "\n"
#define all(a) a.begin(),a.end()
#define rep(n) for(int i = 0; i < (n); i++)
#define rrep(i,a,b) for(int i = (b) - 1; i >= (a); i--)

template <class S, S (*op)(S, S), S (*e)()> class RangeTree {
private:
  int n;
  vector<i64> pts;
  vector<S> d;
public:
  void add_point(i64 x) { pts.emplace_back(x); }
  void build() {
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());
    n = pts.size();
    d.resize(2*n);
    rep (n) d[n+i] = e();
    rrep (i, 1, n) d[i] = op(d[2*i], d[2*i+1]);
  }
  // 座標x,yに値valを置く
  void set(i64 x, S val) {
    int i = lower_bound(all(pts), x) - pts.begin();
    assert(i < n and pts[i] == x);
    d[i+n] = val;
    for (i>>=1; i; i>>=1) d[i] = op(d[2*i], d[2*i+1]);
  }
  // 座標x,yに値valを重ねて置く
  void add(i64 x, S val) {
    int i = lower_bound(all(pts), x) - pts.begin();
    assert(i < n and pts[i] == x);
    for (i+=n; i; i>>=1) d[i] = op(d[i], val);
  }
  // [xl, xr)
  S prod(i64 xl, i64 xr) {
    int l = n + (lower_bound(all(pts), xl) - pts.begin());
    int r = n + (lower_bound(all(pts), xr) - pts.begin());
    S ret = e();
    while (l < r) {
      if (l & 1) ret = op(ret, d[l++]);
      if (r & 1) ret = op(ret, d[--r]);
      l >>= 1, r >>= 1;
    }
    return ret;
  }
  S get(i64 x) { return prod(x, x+1); }
};

template <class S, S (*op)(S, S), S (*e)()> class RangeTree2d {
private:
  int n;
  using pt = pair<i64, i64>;
  vector<pt> pts;
  vector<RangeTree<S, op, e>> rts;

public:
  void add_point(i64 x, i64 y) { pts.emplace_back(x, y); }
  void build() {
    sort(all(pts));
    pts.erase(unique(all(pts)), pts.end());
    n = pts.size();
    rts.resize(2*n);
    vector<vector<i64>> d(2*n);
    rep (n) {
      d[n+i] = {pts[i].second};
      rts[n+i].add_point(pts[i].second);
    }
    rrep (i, 1, n) {
      auto &lch = d[i*2];
      auto &rch = d[i*2+1];
      merge(all(lch), all(rch), back_inserter(d[i]));
      for (i64 &y: d[i]) rts[i].add_point(y);
    }
    for (RangeTree<S, op, e> &rt: rts) rt.build();
  }
  // 座標x,yに値valを置く
  void set(i64 x, i64 y, S val) {
    int i = lower_bound(all(pts), pt{x, y}) - pts.begin();
    assert(i < n and pts[i] == make_pair(x, y));
    for (i+=n; i; i>>=1) rts[i].set(y, val);
  }
  // 座標x,yに値valを重ねて置く
  void add(i64 x, i64 y, S val) {
    int i = lower_bound(all(pts), pt{x, y}) - pts.begin();
    assert(i < n and pts[i] == make_pair(x, y));
    for (i+=n; i; i>>=1) rts[i].add(y, val);
  }
  // [xl, xr), [yl, yr)
  S prod(i64 xl, i64 yl, i64 xr, i64 yr) {
    auto cmp = [](const pt &l, const pt &r) { return l.first < r.first; };
    int l = n + (lower_bound(all(pts), pt{xl, 0}, cmp) - pts.begin());
    int r = n + (lower_bound(all(pts), pt{xr, 0}, cmp) - pts.begin());
    S ret = e();
    while (l < r) {
      if (l & 1) ret = op(ret, rts[l++].prod(yl, yr));
      if (r & 1) ret = op(ret, rts[--r].prod(yl, yr));
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
  RangeTree2d<i64, op, e> rt2d;
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