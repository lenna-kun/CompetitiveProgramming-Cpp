#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t
#define endl "\n"
#define rep(n) for(int i = 0; i < (n); i++)

template <class S, S (*op)(S, S), S (*e)()> struct Node {
  Node<S, op, e> *l, *r, *p;
  i64 pt;
  S v, prod_st;
  explicit Node(i64 pt_, S v_)
    : l(nullptr), r(nullptr), p(nullptr), pt(pt_), v(v_), prod_st(v_) {}
  int state() {
    if (p && p->l == this) return -1;
    if (p && p->r == this) return 1;
    return 0;
  }
  S get_lprod() {
    if (!l) return e();
    return l->prod_st;
  }
  S get_rprod() {
    if (!r) return e();
    return r->prod_st;
  }
  void update() {
    prod_st = op(op(get_lprod(), v), get_rprod());
  }
  void rotate() {
    Node<S, op, e> *par = p;
    Node<S, op, e> *mid;
    if (p->l == this) {
      mid = r; r = par;
      par->l = mid;
    } else {
      mid = l; l = par;
      par->r = mid;
    }
    if (mid) mid->p = par;
    p = par->p; par->p = this;
    if (p && p->l == par) p->l = this;
    if (p && p->r == par) p->r = this;
    par->update(); update();
  }
  void splay() {
    while(state()) {
      int st = state() * p->state();
      if (st == 0) {
        rotate();
      } else if (st == 1) {
        p->rotate();
        rotate();
      } else {
        rotate();
        rotate();
      }
    }
  }
};

template <class S, S (*op)(S, S), S (*e)()> struct RangeBST {
private:
  using NC = Node<S, op, e>;
  NC *root, *max_node;
  NC *build(int li, int ri, const vector<pair<i64, S>> &v) {
    if (li >= ri) return nullptr;
    int mi = (li + ri) >> 1;
    NC *mid = new NC(v[mi].first, v[mi].second);
    mid->l = build(li, mi, v);
    if (mid->l) mid->l->p = mid;
    mid->r = build(mi+1, ri, v);
    if (mid->r) mid->r->p = mid;
    mid->update();
    return mid;
  }
  NC* bound(i64 x, bool lower) {
    NC *left = root, *right = nullptr;
    while (left) {
      if ((lower && !(x > left->pt)) || (!lower && (x < left->pt))) {
        right = left;
        left = left->l;
      } else {
        left = left->r;
      }
    }
    return right;
  }
  void set(i64 x, S val, bool add) {
    NC *node = nullptr;
    if (max_node && max_node->pt >= x) node = lower_bound(x);
    if (node && node->pt == x) {
      if (add) node->v = op(node->v, val);
      else node->v = val;
      node->update();
      return;
    }
    NC *nn = new NC(x, val);
    if (!max_node) {
      max_node = nn;
      root = nn;
      return;
    }
    if (!node) {
      nn->l = root;
      max_node = nn;
    } else {
      // now node->pt > x
      nn->l = node->l; nn->r = node;
      node->l = nullptr; node->p = nn;
      node->update();
    }
    nn->update();
    if (nn->l) nn->l->p = nn;
    root = nn;
    return;
  }
public:
  RangeBST() : root(nullptr), max_node(nullptr) {}
  explicit RangeBST(const vector<pair<i64, S>> &v) : RangeBST() {
    root = build(0, v.size(), v);
    max_node = root;
    while (max_node->r) max_node = max_node->r;
  }
  NC *rbegin() { return max_node; }
  NC* lower_bound(i64 x) {
    NC *ret = bound(x, true);
    if (ret) ret->splay(), root = ret;
    return ret;
  }
  NC* upper_bound(i64 x) {
    NC *ret = bound(x, false);
    if (ret) ret->splay(), root = ret;
    return ret;
  }
  S get(i64 x) {
    NC *ret = lower_bound(x);
    if (!ret || ret->pt != x) return e();
    return ret->v;
  }
  void set(i64 x, S val) { set(x, val, false); }
  void add(i64 x, S val) { set(x, val, true); }
  S prod(i64 xl, i64 xr) {
    assert(xl <= xr);
    if (!lower_bound(xl)) return e();
    if (xr > max_node->pt) return op(root->v, root->get_rprod());
    // now xl is root
    NC *right = bound(xr, true);
    NC *tmp = right;
    S ret = e();
    for (bool f = true; tmp != root;) {
      if (f) ret = op(tmp->get_lprod(), ret);
      f = tmp->state() == 1;
      if (f) ret = op(tmp->p->v, ret);
      tmp = tmp->p;
    }
    if (right) right->splay(), root = right;
    return ret;
  }
};

// verify: https://judge.yosupo.jp/submission/75441
i64 op(i64 a, i64 b) { return a + b; }
i64 e() { return 0; }
void Main() {
  int n, q;
  cin >> n >> q;
  vector<pair<i64, i64>> a(n);
  rep (n) a[i].first = i, cin >> a[i].second;
  RangeBST<i64, op, e> rbst(a);
  rep (q) {
    int t;
    cin >> t;
    if (t == 0) {
      i64 p, x;
      cin >> p >> x;
      rbst.add(p, x);
    } else {
      i64 l, r;
      cin >> l >> r;
      cout << rbst.prod(l, r) << endl;
    }
  }
}
int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout << setprecision(10) << fixed;
  Main();
}