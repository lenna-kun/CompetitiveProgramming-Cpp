#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
template <class T> bool chmax(T& a, T b){ if(a < b){ a = b; return 1; } return 0; }

template <class S, S (*op)(S, S), S (*e)()> class HLD {
public:
  HLD() : HLD(vector<vector<int>>(0), -1) {}
  explicit HLD(vector<vector<int>> &tree, int root_)
    : HLD(tree, root_, vector<S>(int(tree.size()), e())) {}
  explicit HLD(vector<vector<int>> &tree, int root_, const vector<S> &v)
    : n(int(tree.size())), root(root_) {
    build_(tree, v);
  }
  void build(vector<vector<int>> &tree, int root_) {
    build(tree, root_, vector<S>(n, e()));
  }
  void build(vector<vector<int>> &tree, int root_, const vector<S> &v) {
    n = int(tree.size());
    root = root_;
    build_(tree, v);
  }
  // set element to node
  void set(int node, S x) {
    stf.set(node_p[node].first, x);
    stb.set(node_p[node].second, x);
  }
  // lca
  int lca(int ln, int rn) {
    while (1) {
      int topl = top[ln], topr = top[rn];
      if (topl == topr) {
        if (depth[ln] < depth[rn]) return ln;
        else return rn;
      }
      if (depth[topl] > depth[topr]) ln = parent[topl];
      else rn = parent[topr];
    }
  }
  // path query
  S query(int ln, int rn) {
    S prodl = e(), prodr = e();
    while (1) {
      int topl = top[ln], topr = top[rn];
      if (topl == topr) {
        if (node_p[ln] < node_p[rn]) { // rn is deeper than ln
          prodl = op(prodl, stf.prod(node_p[ln].first, node_p[rn].first+1));
        } else { // ln is deeper than rn
          prodl = op(prodl, stb.prod(node_p[ln].second, node_p[rn].second+1));
        }
        return op(prodl, prodr);
      }
      if (depth[topl] > depth[topr]) {
        prodl = op(prodl, stb.prod(node_p[ln].second, node_p[topl].second+1));
        ln = parent[topl];
      } else {
        prodr = op(stf.prod(node_p[topr].first, node_p[rn].first+1), prodr);
        rn = parent[topr];
      }
    }
  }
private:
  int n, root;
  segtree<S, op, e> stf, stb;
  vector<int> parent, top, depth;
  vector<pair<int, int>> node_p;
  void build_(vector<vector<int>> &tree, const vector<S> &v) {
    assert(n == int(v.size())); assert(root < n);
    // create segtree
    stf = segtree<S, op, e>(n);
    stb = segtree<S, op, e>(n);

    parent = vector<int>(n);
    top = vector<int>(n);
    node_p = vector<pair<int, int>>(n);
    depth = vector<int>(n);
    vector<int> heavy(n, -1);
    preprocess1(tree, heavy, root, root, 0);
    parent[root] = -1;
    int tour_index = 0;
    preprocess2(tree, v, heavy, tour_index, root, root, root);
  }
  // set heavy node, parent, depth
  int preprocess1(vector<vector<int>> &tree, vector<int> &heavy, int node, int p, int d) {
    parent[node] = p;
    depth[node] = d;
    int size = 1, size_max = 0, size_c;
    for (int &c: tree[node]) {
      if (c == p) continue;
      size_c = preprocess1(tree, heavy, c, node, d+1); // get child's size
      size += size_c; // update size
      if (chmax(size_max, size_c)) heavy[node] = c; // update heavy node
    }
    return size;
  }
  // build segtree, set top
  int preprocess2(vector<vector<int>> &tree, const vector<S> &v, vector<int> &heavy, int tour_index, int node, int p, int t) {
    node_p[node].first = tour_index;
    stf.set(tour_index++, v[node]);
    node_p[node].second = n - tour_index;
    stb.set(n - tour_index, v[node]);
    top[node] = t; // set top
    if (heavy[node] >= 0) { // heavy node and depth first search
      tour_index = preprocess2(tree, v, heavy, tour_index, heavy[node], node, t);
    }
    for (int &c: tree[node]) {
      if (c == p || c == heavy[node]) continue;
      tour_index = preprocess2(tree, v, heavy, tour_index, c, node, c); // next node is top
    }
    return tour_index;
  }
};