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

i64 modpow(i64 a, i64 b, i64 m) {
	i64 p = 1, q = a;
	for (int i = 0; i < 63; i++) {
		if ((b & (1LL << i)) != 0) {
			p *= q;
			p %= m;
		}
		q *= q;
		q %= m;
	}
	return p;
}

pair<vector<i64>, vector<i64>> init(i64 n, i64 m) {
  vector<i64> fact(n+1, 1), rfact(n+1, 1);
  i64 r = 1;
  rep (i, 1, n+1)
    fact.at(i) = r = (r * i) % m;
  rfact.at(n) = r = modpow(fact.at(n), m-2, m);
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