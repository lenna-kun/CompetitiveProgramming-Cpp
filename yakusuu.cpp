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

vector<i64> eratosthenes(i64 n) {
  vector<i64> prime;
  i64 limit = (i64)sqrt(n) + 1;
  vector<i64> data(n-1);
  rep (n-1) data[i] = i+2;
  while (1) {
    i64 p = data[0];
    if (limit < p) {
      prime.reserve(prime.size() + distance(data.begin(), data.end()));
      prime.insert(prime.end(), data.begin(), data.end());
      return prime;
    }
    prime.push_back(p);
    vector<i64> data_n;
    for (i64 &e: data) {
      if (e % p != 0) data_n.push_back(e);
    }
    swap(data, data_n);
  }
}

void Main() {
  vector<i64> data = eratosthenes(100);
  for (i64 &e: data) cout << e << " ";
  cout << endl;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout << setprecision(10) << fixed;
  Main();
}

// # 素因数分解の前準備
// def sieve(n):
//   data = [2, 0]*(n//2+5)
//   for x in range(3, n+1, 2):
//     if data[x] == 0:
//       data[x] = x
//       if x**2 > n: continue
//       for y in range(x**2, n+5, 2*x):
//         if data[y] == 0:
//           data[y] = x
//   return data
 
// # 素因数分解
// def pfct(data, n): # O(log(n))
//   ret = []
//   while n > 1:
//     if ret and data[n] == ret[-1][0]:
//       ret[-1][1] += 1
//     else:
//       ret.append([data[n], 1])
//     n //= data[n]
//   return ret

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

// # 約数全列挙
// def yakusuu(n): # O(sqrt(n))
//   ret = []
//   for i in range(1, n+1):
//     if i * i > n:
//       break
//     if n % i == 0:
//       ret.append(i)
//       if i * i != n:
//         ret.append(n // i)
//   ret.sort()
//   return ret

// print(yakusuu(10))
// # print(eratosthenes(4))
// print(factorization(72))