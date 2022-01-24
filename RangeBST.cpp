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
  NC *root, *min_, *max_;
  void splay(NC *node) { node->splay(), root = node; }
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
    NC *valid = nullptr, *left = root, *right = nullptr;
    while (left) {
      valid = left;
      if ((lower && !(x > left->pt)) || (!lower && (x < left->pt))) {
        right = left;
        left = left->l;
      } else left = left->r;
    }
    if (!right && valid) splay(valid);
    return right;
  }
  void set(i64 x, S val, bool add) {
    NC *nn = new NC(x, val);
    // if no nodes in tree
    if (!root) {
      min_ = nn, max_ = nn, root = nn; return;
    } if (min_->pt > x) { // if x become min key in tree
      min_->l = nn, nn->p = min_, min_ = nn;
      splay(nn); return;
    } if (max_->pt < x) { // if x become max key in tree
      max_->r = nn, nn->p = max_, max_ = nn;
      splay(nn); return;
    }
    NC *node = bound(x, true); // assert node is not null
    if (node->pt == x) { // if tree already has key x
      if (add) node->v = op(node->v, val);
      else node->v = val;
      node->update(); splay(node); delete nn; return;
    }
    // now node is first node whose key is larger than x
    nn->l = node->l; node->l = nn;
    nn->p = node; if (nn->l) nn->l->p = nn;
    nn->update(); splay(nn);
  }
public:
  RangeBST() : root(nullptr), min_(nullptr), max_(nullptr) {}
  explicit RangeBST(const vector<pair<i64, S>> &v) : RangeBST() {
    root = build(0, v.size(), v);
    for (max_ = root; max_->r;) max_ = max_->r;
    for (min_ = root; min_->l;) min_ = min_->l;
  }
  NC* lower_bound(i64 x) {
    NC *ret = bound(x, true);
    if (ret) splay(ret);
    return ret;
  }
  NC* upper_bound(i64 x) {
    NC *ret = bound(x, false);
    if (ret) splay(ret);
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
    if (!root || xl > max_->pt || xr <= min_->pt) return e();
    if (xl <= min_->pt && xr > max_->pt) return root->prod_st;
    if (xl <= min_->pt) return lower_bound(xr)->get_lprod();
    lower_bound(xl); // now xl is root
    if (xr > max_->pt) return op(root->v, root->get_rprod());
    NC *right = bound(xr, true);
    NC *tmp = right;
    S ret = e();
    for (bool f = true; tmp != root;) {
      if (f) ret = op(tmp->get_lprod(), ret);
      f = tmp->state() == 1;
      if (f) ret = op(tmp->p->v, ret);
      tmp = tmp->p;
    }
    if (right) splay(right);
    return ret;
  }
};

// verify: https://atcoder.jp/contests/abc231/submissions/28770931
ii64 op(i64 a, i64 b) { return a + b; }
i64 e() { return 0; }
void Main() {
  int n;
  cin >> n;
  vector<pair<i64, i64>> present(n);
  rep (n) {
    i64 a;
    cin >> a;
    present[i].first = a;
  }
  rep (n) {
    i64 b;
    cin >> b;
    present[i].second = -b;
  }
  sort(all(present));
  vector<pair<pair<i64, i64>, int>> presents;
  rep (n) {
    if (i == 0 || present[i] != present[i-1])
      presents.push_back({present[i], 1});
    else
      presents.back().second++;
  }
  RangeBST<i64, op, e> rbst;
  i64 ans = 0;
  for (pair<pair<i64, i64>, int> &e: presents) {
    ans += (rbst.prod(-e.first.second, inf) + e.second) * e.second;
    rbst.add(-e.first.second, e.second);
  }
  cout << ans << endl;
}
 
int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cout << setprecision(10) << fixed;
  Main();
}