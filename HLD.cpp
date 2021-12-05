#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
template <class T> bool chmax(T& a, T b){ if(a < b){ a = b; return 1; } return 0; }

class Node {
public:
  int depth, parent, top, indexf, indexb;
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
    build(tree, root_, vector<S>(n, e()));
  }
  void build(vector<vector<int>> &tree, int root_, const vector<S> &v) {
    n = int(tree.size());
    root = root_;
    build_(tree, v);
  }
  // set element to node
  void set(int node, S x) {
    stf.set(nodes[node].indexf, x);
    stb.set(nodes[node].indexb, x);
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
          prodl = op(prodl, stf.prod(nodes[ln].indexf, nodes[rn].indexf+1));
        } else { // ln is deeper than rn
          prodl = op(prodl, stb.prod(nodes[ln].indexb, nodes[rn].indexb+1));
        }
        return op(prodl, prodr);
      }
      if (nodes[topl].depth > nodes[topr].depth) {
        prodl = op(prodl, stb.prod(nodes[ln].indexb, nodes[topl].indexb+1));
        ln = nodes[topl].parent;
      } else {
        prodr = op(stf.prod(nodes[topr].indexf, nodes[rn].indexf+1), prodr);
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
    preprocess1(tree, heavy, root, root, 0);
    preprocess2(tree, v, heavy, 0, root, root, root);
  }
  // set heavy node, parent, depth
  int preprocess1(vector<vector<int>> &tree, vector<int> &heavy, int node, int p, int d) {
    nodes[node].parent = p;
    nodes[node].depth = d;
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
    nodes[node].indexf = tour_index;
    stf.set(tour_index++, v[node]);
    nodes[node].indexb = n - tour_index;
    stb.set(n - tour_index, v[node]);
    nodes[node].top = t; // set top
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