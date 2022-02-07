#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t
#define endl "\n"
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
template <
  class S, void (*add)(int), void (*del)(int), S (*solve)()
> struct Mo {
private:
  int q;
  vector<pair<int, int>> qs;
  vector<int> qi;
public:
  explicit Mo(vector<pair<int, int>> &q_) : q(q_.size()), qs(q_) {
    qi.resize(q);
    iota(all(qi), 0);
    vector<i64> eval(q);
    rep (q) eval[i] = hilbertorder(qs[i].first, qs[i].second);
    sort(all(qi), [&](int i, int j) {
      return eval[i] < eval[j];
    });
  }

  vector<S> execute() {
    vector<S> ret(q);
    int nl = 0, nr = 0;
    for (int i: qi) {
      while (nl > qs[i].first) add(--nl);
      while (nr < qs[i].second) add(nr++);
      while (nl < qs[i].first) del(nl++);
      while (nr > qs[i].second) del(--nr);
      ret[i] = solve();
    }
    return move(ret);
  }
};

struct Osa_k {
private:
  vector<int> min_factor;
public:
  Osa_k(int n){
    //素数候補を管理する配列
    min_factor = vector<int>(n+1);
    //2以上の数を一度素数候補に入れる
    rep (n+1) min_factor[i] = i;
    //それぞれの数について最小の素因数を求める
    for (int i=2; i*i <= n; i++){
      if (min_factor[i] != i) continue;
      for(int j=2; i*j <= n; j++)
        if(min_factor[i*j] > i) min_factor[i*j] = i;
    }
  }

  vector<pair<int, int>> prime_division(i64 n) { // O(log n)
    vector<pair<int, int>> ret;
    while (n > 1) {
      if (!ret.empty() && min_factor[n] == ret.back().first)
        ret.back().second++;
      else
        ret.push_back({min_factor[n], 1});
      n /= min_factor[n];
    }
    return ret;
  }
};

vector<vector<pair<int, int>>> pd;
vector<int> prime_cnt(1000010);
int cnt = 0;

void add(int i) {
  for (pair<int, int> &p: pd[i]) {
    if (prime_cnt[p.first] == 0) cnt++;
    prime_cnt[p.first] = (prime_cnt[p.first] + p.second)%3;
    if (prime_cnt[p.first] == 0) cnt--;
  }
}

void del(int i) {
  for (pair<int, int> &p: pd[i]) {
    if (prime_cnt[p.first] == 0) cnt++;
    prime_cnt[p.first] = (prime_cnt[p.first] - p.second)%3;
    if (prime_cnt[p.first] == 0) cnt--;
  }
}

bool solve() {
  return cnt == 0;
}

void Main() {
  Osa_k osa_k(1000010);
  int n, q;
  cin >> n >> q;
  pd.resize(n);
  vector<int> a(n);
  rep (n) {
    cin >> a[i];
    pd[i] = osa_k.prime_division(a[i]);
  }
  vector<pair<int, int>> qs(q);
  for (pair<int, int> &e: qs) {
    cin >> e.first >> e.second;
    e.first--;
  }
  Mo<bool, add, del, solve> mo(qs);
  for (bool ans: mo.execute()) {
    if (ans) cout << "Yes" << endl;
    else cout << "No" << endl;
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout << setprecision(10) << fixed;
  Main();
}