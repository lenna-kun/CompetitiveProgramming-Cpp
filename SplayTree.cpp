#include <bits/stdc++.h>
using namespace std;

template <class T> struct Node {
  Node<T> *l, *r, *p;
  T k;
  Node(T k_) : l(nullptr), r(nullptr), p(nullptr), k(k_) {}
  int state() {
    if (p && p->l == this) return -1;
    if (p && p->r == this) return 1;
    return 0;
  }
  void rotate() {
    Node<T> *par = p;
    Node<T> *mid;
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
  Node<T>* next(bool asc) {
    if (asc ? r : l) {
      Node<T> *cur = asc ? r : l;
      if (asc) while (cur->l) cur = cur->l;
      else while (cur->r) cur = cur->r;
      return cur;
    }
    Node<T> *cur = this;
    while (cur->state() == (asc ? 1 : -1)) cur = cur->p;
    return cur->p;
  }
};

template <class T> struct SplayTree {
private:
  using NC = Node<T>;
  NC *root, *min_, *max_;
  int sz;
  void splay(NC *node) { node->splay(), root = node; }
  NC* bound(T k, bool lower) {
    NC *valid = nullptr, *left = root, *right = nullptr;
    while (left) {
      valid = left;
      if ((lower && !(k > left->k)) || (!lower && (k < left->k))) {
        right = left;
        left = left->l;
      } else left = left->r;
    }
    if (!right && valid) splay(valid);
    return right;
  }
public:
  SplayTree() : root(nullptr), min_(nullptr), max_(nullptr), sz(0) {}
  int size() { return sz; }
  NC *begin() { return min_; }
  NC *rbegin() { return max_; }
  NC* lower_bound(T k) {
    NC *ret = bound(k, true);
    if (ret) splay(ret);
    return ret;
  }
  NC* upper_bound(T k) {
    NC *ret = bound(k, false);
    if (ret) splay(ret);
    return ret;
  }
  NC* entry(T k) {
    lower_bound(k);
    if (!root || root->k != k) return nullptr;
    return root;
  }
  pair<NC*, bool> insert(T k) {
    NC *nn = new NC(k);
    if (!root) { // if no nodes in tree
      min_ = nn, max_ = nn, root = nn;
    } else if (min_->k > k) { // if k become min k in tree
      min_->l = nn, nn->p = min_, min_ = nn;
      splay(nn);
    } else if (max_->k < k) { // if k become max k in tree
      max_->r = nn, nn->p = max_, max_ = nn;
      splay(nn);
    } else {
      NC *node = bound(k, true); // assert node is not null
      if (node->k == k) { // if tree already has k k
        splay(node); delete nn; return {root, false};
      }
      // now node is first node whose k is larger than k
      nn->l = node->l; node->l = nn;
      nn->p = node; if (nn->l) nn->l->p = nn;
      splay(nn);
    }
    sz++; return {root, true};
  }
  void erase(NC *node) {
    assert(node); splay(node); sz--;
    if (!root->l) { // it means this node is min_
      // if no nodes remain
      if (!root->r) root = nullptr, min_ = nullptr, max_ = nullptr;
      else {
        root = root->r; root->p = nullptr;
        for (min_ = root; min_->l; min_ = min_->l);
        splay(min_);
      }
    } else if (!root->r) {
      root = root->l; root->p = nullptr;
      for (max_ = root; max_->r; max_ = max_->r);
      splay(max_);
    } else {
      root->l->p = nullptr; // cut left tree
      NC *nx_root = root->l; // next root
      for (;nx_root->r; nx_root = nx_root->r);
      splay(nx_root);
      // now nx_root doesn't have right child
      nx_root->r = node->r; nx_root->r->p = nx_root;
      root->p = nullptr;
    }
    delete node;
  }
  bool erase(T k) {
    if (!entry(k)) return false;
    erase(root);
    return true;
  }
};
int main() {
  SplayTree<int> set;
  set.insert(30);
  set.insert(41);
  set.insert(51);
  set.erase(41);
  cout << set.lower_bound(30)->k << endl;
}