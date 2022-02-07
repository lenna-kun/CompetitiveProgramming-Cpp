#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t
#define all(a) a.begin(),a.end()
#define rep(n) for(int i = 0; i < (n); i++)

constexpr int maxn = 1 << 20;
i64 hilbertorder(int x, int y) {
  i64 d = 0;
  for (int s=maxn/2; s; s>>=1) {
    bool rx = x & s, ry = y & s;
    d = d << 2 | rx * 3 ^ static_cast<int>(ry);
    if (ry) continue;
    if (rx) {
      x = maxn - x;
      y = maxn - y;
    }
    swap(x, y);
  }
  return d;
}

// S: クエリの答えのクラス
template <class S> struct Mo {
private:
  int q = 0;
  vector<int> l, r;
public:
  void insert(int l_, int r_) {
    q += 1;
    l.emplace_back(l_);
    r.emplace_back(r_);
  }

  // F1~F5: lambda関数
  template<typename F1, typename F2, typename F3, typename F4, typename F5>
  vector<S> execute(F1 &&add_l, F2 &&add_r, F3 &&del_l, F4 &&del_r, F5 solve) {
    vector<int> qi(q);
    iota(all(qi), 0);
    vector<i64> eval(q);
    rep (q) eval[i] = hilbertorder(l[i], r[i]);
    sort(all(qi), [&](int i, int j) {
      return eval[i] < eval[j];
    });
    vector<S> ret(q);
    int nl = 0, nr = 0;
    for (int i: qi) {
      while (nl > l[i]) add_l(--nl);
      while (nr < r[i]) add_r(nr++);
      while (nl < l[i]) del_l(nl++);
      while (nr > r[i]) del_r(--nr);
      ret[i] = solve();
    }
    return move(ret);
  }
};