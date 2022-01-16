#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
template <class T> bool chmax(T& a, T b){ if(a < b){ a = b; return 1; } return 0; }

class Node {
public:
  int depth, parent, top, idxf, idxb;
  Node() : parent(-1) {}
};

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
    n = int(tree.size());
    build(tree, root_, vector<S>(n, e()));
  }
  void build(vector<vector<int>> &tree, int root_, const vector<S> &v) {
    n = int(tree.size());
    root = root_;
    build_(tree, v);
  }
  // set element to node
  void set(int node, S x) {
    stf.set(nodes[node].idxf, x);
    stb.set(nodes[node].idxb, x);
  }
  // lca
  int lca(int ln, int rn) {
    while (1) {
      int topl = nodes[ln].top, topr = nodes[rn].top;
      if (topl == topr) {
        if (nodes[ln].depth < nodes[rn].depth) return ln;
        else return rn;
      }
      if (nodes[topl].depth > nodes[topr].depth) ln = nodes[topl].parent;
      else rn = nodes[topr].parent;
    }
  }
  // path query
  S query(int ln, int rn) {
    S prodl = e(), prodr = e();
    while (1) {
      int topl = nodes[ln].top, topr = nodes[rn].top;
      if (topl == topr) {
        if (nodes[ln].depth < nodes[rn].depth) { // rn is deeper than ln
          prodl = op(prodl, stf.prod(nodes[ln].idxf, nodes[rn].idxf+1));
        } else { // ln is deeper than rn
          prodl = op(prodl, stb.prod(nodes[ln].idxb, nodes[rn].idxb+1));
        }
        return op(prodl, prodr);
      }
      if (nodes[topl].depth > nodes[topr].depth) {
        prodl = op(prodl, stb.prod(nodes[ln].idxb, nodes[topl].idxb+1));
        ln = nodes[topl].parent;
      } else {
        prodr = op(stf.prod(nodes[topr].idxf, nodes[rn].idxf+1), prodr);
        rn = nodes[topr].parent;
      }
    }
  }
private:
  int n, root;
  segtree<S, op, e> stf, stb;
  vector<Node> nodes;
  void build_(vector<vector<int>> &tree, const vector<S> &v) {
    assert(n == int(v.size())); assert(root < n);
    // create segtree
    stf = segtree<S, op, e>(n);
    stb = segtree<S, op, e>(n);

    nodes = vector<Node>(n);
    vector<int> heavy(n, -1);
    pp1(tree, heavy, root, root, 0);
    pp2(tree, v, heavy, 0, root, root, root);
  }
  // set heavy node, parent, depth
  int pp1(vector<vector<int>> &tree, vector<int> &heavy, int node, int p, int d) {
    nodes[node].parent = p;
    nodes[node].depth = d;
    int sz = 1, sz_max = 0, sz_c;
    for (int &c: tree[node]) {
      if (c == p) continue;
      sz_c = pp1(tree, heavy, c, node, d+1); // get child's size
      sz += sz_c; // update size
      if (chmax(sz_max, sz_c)) heavy[node] = c; // update heavy node
    }
    return sz;
  }
  // build segtree, set top
  int pp2(vector<vector<int>> &tree, const vector<S> &v, vector<int> &heavy, int tour_idx, int node, int p, int t) {
    nodes[node].idxf = tour_idx;
    stf.set(tour_idx++, v[node]);
    nodes[node].idxb = n - tour_idx;
    stb.set(n - tour_idx, v[node]);
    nodes[node].top = t; // set top
    if (heavy[node] >= 0) { // heavy node and depth first search
      tour_idx = pp2(tree, v, heavy, tour_idx, heavy[node], node, t);
    }
    for (int &c: tree[node]) {
      if (c == p || c == heavy[node]) continue;
      tour_idx = pp2(tree, v, heavy, tour_idx, c, node, c); // next node is top
    }
    return tour_idx;
  }
};