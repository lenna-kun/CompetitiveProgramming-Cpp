// #pragma GCC optimize ("O2")
// #pragma GCC optimize("unroll-loops")
// #pragma GCC target ("avx2")
#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t

template<typename _Tp> class BSTNode {
public:
  int from, to, sz;
  _Tp value, al, lazy;
  BSTNode *left, *right, *par;
  BSTNode(const int _from, const int _to) noexcept
    : from(_from), to(_to), sz(0), value(id1), al(id2), lazy(id1),
      left(nullptr), right(nullptr), par(nullptr) {}
  BSTNode(const int _from, const int _to, const _Tp& _value) noexcept
    : from(_from), to(_to), sz(1), value(_value), al(value), lazy(id1),
      left(nullptr), right(nullptr), par(nullptr) {}
  static const _Tp id1 = (_Tp)0;
  static const _Tp id2 = (_Tp)0;
  static void opr1(_Tp& arg1, const _Tp arg2) noexcept { arg1 += arg2; }
  static _Tp opr2(const _Tp arg1, const _Tp arg2) noexcept { return arg1 + arg2; }
  inline bool IsRoot() const noexcept { return !par; }
  inline bool IsVertex() const noexcept { return from == to; }
  void push() noexcept {
    if (lazy != id1) {
      if (IsVertex()) opr1(value, lazy);
      al += lazy * sz;
      if (left) opr1(left->lazy, lazy);
      if (right) opr1(right->lazy, lazy);
      lazy = id1;
    }
  }
  void eval() noexcept {
    sz = IsVertex(), al = value;
    if (left) left->push(), sz += left->sz, al = opr2(left->al, al);
    if (right) right->push(), sz += right->sz, al = opr2(al, right->al);
  }
  void rotate(const bool right_) noexcept { // right rotation or left rotation
    BSTNode *p = par, *g = p->par;
    if (right_) {
      if ((p->left = right)) right->par = p;
      right = p, p->par = this;
    } else {
      if ((p->right = left)) left->par = p;
      left = p, p->par = this;
    }
    p->eval(), eval();
    if (!(par = g)) return;
    if (g->left == p) g->left = this;
    if (g->right == p) g->right = this;
    g->eval();
  }
};

template<typename _Tp>
BSTNode<_Tp>* splay(BSTNode<_Tp> *u) noexcept { // splay until node u becomes the root
  if (!u) return nullptr;
  while (!(u->IsRoot())) {
    BSTNode<_Tp> *p = u->par, *gp = p->par;
    if (p->IsRoot()) { // zig
      p->push(), u->push();
      u->rotate((u == p->left)); // if node u is left child, rotate right. if not, rotate left.
    } else { // zig-(?:zig|zag)
      gp->push(), p->push(), u->push();
      bool flag = (u == p->left); // whether node u is left child or not
      if (flag == (p == gp->left)) { // zig-zig
        p->rotate(flag), u->rotate(flag);
      } else { // zig-zag
        u->rotate(flag), u->rotate(!flag);
      }
    }
  }
  u->push();
  return u;
}

template<typename _Tp>
BSTNode<_Tp>* join(BSTNode<_Tp> *root1, BSTNode<_Tp> *root2) noexcept {
  // assume root1 is the root of tree1, root2 is the root of tree2
  if (!root1 || !root2) return root1 ? root1 : root2;
  BSTNode<_Tp> *cur = nullptr, *nx = root1;
  do { cur = nx, cur->push(), nx = cur->right; } while (nx);
  // now node cur is the node whose index is maximum in tree1
  BSTNode<_Tp> *ver = splay(cur); // splay with the maximum node
  // now the root of tree1 doesn't have right child
  ver->right = root2, ver->eval(), root2->par = ver; // so just link
  return ver; // return the root of the joined tree
}

template<typename _Tp>
pair<BSTNode<_Tp>*, BSTNode<_Tp>*> split_lower_bound(BSTNode<_Tp> *ver) noexcept {
  BSTNode<_Tp> *res = splay(ver)->left;
  ver->left = nullptr, ver->eval();
  if (res) res->par = nullptr;
  return make_pair(res, ver);
}

template<typename _Tp>
pair<BSTNode<_Tp>*, BSTNode<_Tp>*> split_upper_bound(BSTNode<_Tp> *ver) noexcept {
  BSTNode<_Tp> *res = splay(ver)->right;
  ver->right = nullptr, ver->eval();
  if (res) res->par = nullptr;
  return make_pair(ver, res);
}

template<typename T> class EulerTourTree {
private:
  BSTNode<T>** vertex_set;
  map<pair<int, int>, pair<BSTNode<T>*, BSTNode<T>*>> edge_set;
  BSTNode<T> *reroot(BSTNode<T> *ver) noexcept {
    BSTNode<T> *res = splay(ver)->left;
    // res is subtree on splay tree consisting of nodes visited before node ver in euler tour
    if (!res) return ver; // already node ver is root
    ver->left = nullptr, ver->eval();
    while (ver->right) ver->push(), ver = ver->right;
    splay(ver), ver->right = res, ver->eval(), res->par = ver; // bring the subtree to end of splay tree
    return ver;
  }
  void link(BSTNode<T> *ver1, BSTNode<T> *ver2) noexcept {
    BSTNode<T>* e1 = new BSTNode<T>(ver1->from, ver2->from);
    BSTNode<T>* e2 = new BSTNode<T>(ver2->from, ver1->from);
    edge_set[{ver1->from, ver2->from}] = {e1, e2};
    join(join(reroot(ver1), e1), join(reroot(ver2), e2));
  }
  void cut(BSTNode<T> *edge1, BSTNode<T> *edge2) noexcept {
    splay(edge1), splay(edge2);
    BSTNode<T> *p = edge1->par;
    bool _right = (edge1 == edge2->right);
    if (p != edge2) {
      _right = (p == edge2->right);
      p->par = nullptr, edge1->rotate((edge1 == p->left));
    }
    if (edge1->left) edge1->left->par = nullptr;
    if (edge1->right) edge1->right->par = nullptr;
    if (_right) {
      if(edge2->left) edge2->left->par = nullptr;
      join(edge2->left, edge1->right);
    } else {
      if (edge2->right) edge2->right->par = nullptr;
      join(edge1->left, edge2->right);
    }
    // delete edge1; delete edge2;
  }
  bool connected(BSTNode<T> *ver1, BSTNode<T> *ver2) noexcept {
    splay(ver1), splay(ver2);
    return ver1->par;
  }
  T& get(BSTNode<T> *ver) noexcept { return splay(ver)->value; }
  void point_update(BSTNode<T> *ver, const T& val){
    BSTNode<T>::opr1(splay(ver)->value, val);
    ver->al = BSTNode<T>::opr2(ver->al, val);
  }
  void range(BSTNode<T> *edge1, BSTNode<T> *edge2, const T& val) noexcept {
    // assume edge1 and edge2 is linked
    // assume edge1 is the parent of edge2
    auto res1 = split_lower_bound(edge1);
    auto res2 = split_upper_bound(edge2);
    BSTNode<T>::opr1(res2.first->lazy, val);
    join(join(res1.first, res2.first), res2.second);
  }
  T query(BSTNode<T> *edge1, BSTNode<T> *edge2) noexcept {
    auto res1 = split_lower_bound(edge1);
    auto res2 = split_upper_bound(edge2);
    T res = res2.first->al;
    return join(join(res1.first,res2.first), res2.second), res;
  }
  void dfs(const int u, const int p, const BSTNode<T> *cur,
    bool *visit, vector<BSTNode<T>*>& nodes, const vector<vector<int> >& G) noexcept {
    visit[u] = true;
    nodes.push_back(vertex_set[u]);
    for (auto& v : G[u]) {
      if (v != p) {
        BSTNode<T>* e1 = new BSTNode<T>(u, v);
        nodes.push_back(e1);
        dfs(v, u, cur, visit, nodes, G);
        BSTNode<T>* e2 = new BSTNode<T>(v, u);
        if (u < v) edge_set[{u, v}] = {e1, e2};
        else edge_set[{v, u}] = {e2, e1};
        nodes.push_back(e2);
      }
    }
  }
  void bst_build(vector<BSTNode<T>* >& nodes) noexcept {
    int i, n = (int)nodes.size(), st = 2, isolate = ((n % 4 == 1) ? (n-1) : -1);
    while (st <= n) {
      for (i = st-1; i < n; i += 2*st) {
        nodes[i]->left = nodes[i-st/2], nodes[i-st/2]->par = nodes[i];
        if (i+st/2 < n) nodes[i]->right = nodes[i+st/2], nodes[i+st/2]->par = nodes[i];
        else if (isolate >= 0) nodes[i]->right = nodes[isolate], nodes[isolate]->par = nodes[i];
        nodes[i]->eval();
      }
      isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st): isolate);
      st <<= 1;
    }
  }
  void build_forest(const vector<vector<int> >& forest) noexcept {
    bool *visit = new bool[V]();
    for (int i = 0; i < V; i++) {
      if (!visit[i]) {
        vector<BSTNode<T>*> nodes;
        BSTNode<T> *cur = nullptr;
        dfs(i, -1, cur, visit, nodes, forest);
        bst_build(nodes);
      }
    }
    // delete[] visit;
  }
  void build_tree(const int root, const vector<vector<int> >& tree) noexcept {
    bool *visit = new bool[V]();
    vector<BSTNode<T>*> nodes;
    BSTNode<T> *cur = nullptr;
    dfs(root, -1, cur, visit, nodes, tree);
    bst_build(nodes);
    // delete[] visit;
  }

public:
  const int V;
  EulerTourTree(const vector<T>& ver_value) noexcept : V((int)ver_value.size()){
    vertex_set = new BSTNode<T>*[V];
    for (int i = 0; i < V; i++) vertex_set[i] = new BSTNode<T>(i, i, ver_value[i]);
  }
  EulerTourTree(const vector<T>& ver_value, const vector<vector<int> >& forest) noexcept
    : EulerTourTree(ver_value) { build_forest(forest); }
  EulerTourTree(const vector<T>& ver_value, const int root, const vector<vector<int> >& tree) noexcept
    : EulerTourTree(ver_value) { build_tree(root, tree); }
  // ~EulerTourTree(){
  //   for(auto it : edge_set){
  //     delete (it.second).first;
  //     delete (it.second).second;
  //   }
  //   for(int i = 0; i < V; ++i) delete vertex_set[i];
  //   delete[] vertex_set;
  // }
  // 根を node_id にする
  void reroot(const int node_id) noexcept { reroot(vertex_set[node_id]); }
  // 辺(node1_id, node2_id) を追加
  void link(int node1_id, int node2_id) noexcept {
    if (node1_id > node2_id) swap(node1_id, node2_id);
    link(vertex_set[node1_id], vertex_set[node2_id]);
  }
  // 辺(node1_id, node2_id) を削除(逆向きでも問題ない)
  void cut(int node1_id, int node2_id){
    if (node1_id > node2_id) swap(node1_id, node2_id);
    auto it = edge_set.find({node1_id, node2_id});
    assert(it != edge_set.end());
    BSTNode<T> *edge1 = (it->second).first, *edge2 = (it->second).second;
    edge_set.erase(it);
    cut(edge1, edge2);
  }
  // node1_id と node2_id が同じ木(連結成分)に属するか
  bool connected(const int node1_id, const int node2_id) noexcept {
    if (node1_id == node2_id) return true;
    return connected(vertex_set[node1_id], vertex_set[node2_id]);
  }
  // 頂点 ver_id の値を取得
  T& get(const int ver_id) noexcept { return get(vertex_set[ver_id]); }
  // 頂点 ver_id に val を加える
  void point_update(const int ver_id, const T& val) noexcept {
    return point_update(vertex_set[ver_id], val);
  }
  // 頂点 ver_id の存在する連結成分内の頂点全体に val を加える
  void component_range(const int ver_id, const T& val){ range(ver_id, -1, val); }
  // 親が par_id であるような頂点 ver_id の部分木内に存在する頂点全体に val を加える
  void range(const int ver_id, const int par_id, const T& val){
    if (par_id < 0) return BSTNode<T>::opr1(splay(vertex_set[ver_id])->lazy, val);
    if (ver_id < par_id) {
      auto it = edge_set.find({ver_id, par_id});
      assert(it != edge_set.end());
      range((it->second).second, (it->second).first, val);
    } else {
      auto it = edge_set.find({par_id, ver_id});
      assert(it != edge_set.end());
      range((it->second).first, (it->second).second, val);
    }
  }
  // 頂点 ver_id の存在する連結成分内の頂点全体の総和を取得
  T component_query(const int ver_id){ return query(ver_id, -1); }
  // 親が par_id であるような頂点 ver_id の部分木内に存在する頂点全体の総和を取得
  T query(const int ver_id, const int par_id){
    if (par_id < 0) return splay(vertex_set[ver_id])->al;
    if (ver_id < par_id) {
      auto it = edge_set.find({ver_id, par_id});
      assert(it != edge_set.end());
      return query((it->second).second, (it->second).first);
    } else {
      auto it = edge_set.find({par_id, ver_id});
      assert(it != edge_set.end());
      return query((it->second).first, (it->second).second);
    }
  }
  // DEBUG 用
  void check_dfs(const BSTNode<T>* cur) const noexcept {
    cur->push();
    if (cur->left) check_dfs(cur->left);
    if (cur->IsVertex()) cout <<"{" << (cur->from) << "," << (cur->value) << "} ";
    if (cur->right) check_dfs(cur->right);
  }
};