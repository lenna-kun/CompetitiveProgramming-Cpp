#include <bits/stdc++.h>
using namespace std;
#define rep(n) for(int i = 0; i < (n); i++)

template <typename T>
struct BinaryIndexedTree {
  int n;
  int floor_pow2;
  vector<T> data;

  BinaryIndexedTree(int size) {
    n = ++size;
    data.assign(n, 0);
    for (floor_pow2 = 1; (floor_pow2 << 1) <= n; floor_pow2 <<= 1);
  }

  // get sum of [0,k]
  T sum(int k) const {
    if (k < 0) return 0;
    T ret = 0;
    for (++k; k > 0; k -= k&(-k)) ret += data[k];
    return ret;
  }

  // getsum of [l,r]
  inline T sum(int l, int r) const { return sum(r) - sum(l-1); }

  // get value of k
  inline T operator[](int k) const { return sum(k) - sum(k-1); }

  // data[k] += x
  void add(int k, T x) {
    for (++k; k < n; k += k&(-k)) data[k] += x;
  }

  // range add x to [l,r]
  void imos(int l, int r, T x) {
    add(l, x);
    add(r+1, -x);
  }

  // minimize i s.t. sum(i) >= w
  int lower_bound(T w) {
    if (w <= 0) return 0;
    int x = 0;
    for (int k = floor_pow2; k > 0; k >>= 1) {
      if (x + k < n && data[x + k] < w) {
        w -= data[x + k];
        x += k;
      }
    }
    return x;
  }

  // minimize i s.t. sum(i) > w
  int upper_bound(T w) {
    if (w < 0) return 0;
    int x = 0;
    for (int k = floor_pow2; k > 0; k >>= 1) {
      if (x + k < n && data[x + k] <= w) {
        w -= data[x + k];
        x += k;
      }
    }
    return x;
  }
};