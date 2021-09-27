#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t

struct union_find {
  i64 groups;
  vector<i64> parents;

  union_find(i64 n) {
    groups = n;
    parents = vector<i64>(n, -1);
  }

  i64 find(i64 x) {
    if (parents.at(x) < 0) {
      return x;
    } else {
      parents[x] = find(parents[x]);
      return parents[x];
    }
  }

  void unite(i64 x, i64 y) {
    x = find(x);
    y = find(y);

    // already united
    if (x == y)
      return;
    
    groups--;

    if (parents[x] > parents[y])
      swap(x, y);
    
    parents[x] += parents[y];
    parents[y] = x;
  }

  i64 size(i64 x) {
    return -parents[find(x)];
  }

  bool issame(i64 x, i64 y) {
    return find(x) == find(y);
  }

  vector<i64> roots() {
    vector<i64> ret;
    for (i64 i = 0; i < parents.size(); i++)
      if (parents[i] < 0)
        ret.push_back(i);
    return ret;
  }

  i64 group_count() {
    return groups;
  }
};