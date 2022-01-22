#include <bits/stdc++.h>
using namespace std;
#define i64 int64_t

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
  RangeBST() : root(nullptr), max_node(nullptr), sz(0) {}
  int size() { return sz; }
  Node<S, op, e> *rbegin() { return max_node; }
  Node<S, op, e>* lower_bound(i64 x) {
    Node<S, op, e> *ret = bound(x, true);
    if (ret) ret->splay(), root = ret;
    return ret;
  }
  Node<S, op, e>* upper_bound(i64 x) {
    Node<S, op, e> *ret = bound(x, false);
    if (ret) ret->splay(), root = ret;
    return ret;
  }
  S get(i64 x) {
    Node<S, op, e> *ret = lower_bound(x);
    if (!ret || ret->pt != x) return e();
    return ret->v;
  }
  void set(i64 x, S val) {
    Node<S, op, e> *node = lower_bound(x);
    if (node && node->pt == x) {
      node->v = val;
      node->update();
      return;
    }
    root = new Node<S, op, e>(x, val);
    sz++;
    if (!max_node) {
      max_node = root;
      return;
    }
    if (!node) {
      max_node->splay();
      root->l = max_node;
      max_node = root;
    } else {
      // now node->pt > x
      root->l = node->l; root->r = node;
      node->l = nullptr; node->p = root;
      node->update();
    }
    root->update();
    if (root->l) root->l->p = root;
    return;
  }
  S prod(i64 xl, i64 xr) {
    assert(xl <= xr);
    if (!lower_bound(xl)) return e();
    // now xl is root
    Node<S, op, e> *right = bound(xr, true);
    if (!right) return op(root->v, root->get_rprod());
    Node<S, op, e> *tmp = right;
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
private:
  Node<S, op, e> *root, *max_node;
  int sz;
  Node<S, op, e>* bound(i64 x, bool lower) {
    Node<S, op, e> *left = root, *right = nullptr;
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
};

i64 op(i64 a, i64 b) { return a + b; }
i64 e() { return 0; }
int main() {
  RangeBST<i64, op, e> rbst;
  rbst.set(30, 90);
  rbst.set(51, 120);
  rbst.set(44, 10);
  cout << rbst.prod(40, 52) << endl;
}