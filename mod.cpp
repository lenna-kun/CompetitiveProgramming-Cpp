#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
#define i64 int64_t
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

pair<vector<i64>, vector<i64>> init(i64 n, i64 m) {
  vector<i64> fact(n+1, 1), rfact(n+1, 1);
  i64 r = 1;
  rep (i, 1, n+1)
    fact.at(i) = r = (r * i) % m;
  rfact.at(n) = r = pow_mod(fact.at(n), m-2, m);
  rrep (i, 1, n+1)
    rfact.at(i-1) = r = (r * i) % m;
  return make_pair(fact, rfact);
}

// nPk (mod MOD)
i64 perm(pair<vector<i64>, vector<i64>> &data, i64 n, i64 k, i64 m) {
  return (data.first.at(n) * data.second.at(n-k)) % m;
}

// nCk (mod MOD)
i64 comb(pair<vector<i64>, vector<i64>> &data, i64 n, i64 k, i64 m) {
  return (((data.first.at(n) * data.second.at(k)) % m) * data.second.at(n-k)) % m;
}

// nHk (mod MOD)
i64 homb(pair<vector<i64>, vector<i64>> &data, i64 n, i64 k, i64 m) {
  return comb(data, n+k-1, k, m);
}

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

// 素因数分解
vector<pair<i64, int>> prime_division(i64 n) { // O(rt(n))
  vector<pair<i64, int>> ret;
  int x = n, cnt;
  for (int i=2; i*i<=n; i++) {
    cnt = 0;
    while (x%i == 0) {
      x /= i;
      cnt++;
    }
    if (cnt) ret.push_back({i, cnt});
  }
  if (x != 1) ret.push_back({x, 1});
  return move(ret);
}

i64 euler_phi(i64 n) {
  i64 phi = n;
  for (int i=2; i*i<=n; i++) {
    if (n%i == 0) {
      phi -= phi/i;
      while (n%i == 0) n /= i;
    }
  }
  if (n > 1) phi -= phi/n;
  return phi;
}

i64 euler_phi2(i64 n) {
  vector<pair<i64, int>> pds = prime_division(n);
  i64 phi = 1;
  for (pair<i64, int> &pd: pds)
    phi *= (pd.first-1) * pow(pd.first, pd.second-1);
  return phi;
}

i64 power_tower(vector<i64> &exps, i64 mod) {
  vector<i64> mods = {mod};
  rep (exps.size()-1) mods.push_back(euler_phi(mods.back()));

  i64 ret = 1;
  rrep (exps.size()) {
    i64 m = mods.back(); mods.pop_back();
    if (ret < 10) {
      i64 tmp = pow(exps[i], ret);
      if (tmp < 64) ret = tmp;
      else ret = 64 + (tmp - 64) % m;
    } else if (exps[i] == 0) {
      ret = 0;
    } else {
      ret = pow_mod(exps[i], ret, m) + 64 * m;
    }
  }
  return ret % mod;
}

int main() {
  int n, m;
  cin >> n >> m;
  vector<i64> a(n);
  for (i64 &e: a) cin >> e;
  cout << power_tower(a, m) << endl;
}