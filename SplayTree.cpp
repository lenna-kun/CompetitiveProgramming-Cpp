#include <bits/stdc++.h>
using namespace std;

template <class T> struct Node {
  Node<T> *l, *r, *p;
  T key;
  Node(T key_) : l(nullptr), r(nullptr), p(nullptr), key(key_) {}
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
  Node<T>* next() {
    if (r) {
      Node<T> *cur = r;
      while (cur->l) cur = cur->l;
      return cur;
    }
    Node<T> *cur = this;
    while (cur->state() == 1) cur = cur->p;
    return cur->p;
  }
  Node<T>* pre() {
    if (l) {
      Node<T> *cur = l;
      while (cur->r) cur = cur->r;
      return cur;
    }
    Node<T> *cur = this;
    while (cur->state() == -1) cur = cur->p;
    return cur->p;
  }
};

template <class T> struct SplayTree {
  SplayTree() : root(nullptr), beg(nullptr), rbeg(nullptr), sz(0) {}
  int size() { return sz; }
  Node<T> *begin() { return beg; }
  Node<T> *rbegin() { return rbeg; }
  Node<T>* end() { return nullptr; }
  Node<T>* rend() { return nullptr; }
  Node<T>* lower_bound(T k) { return bound(k, true); }
  Node<T>* upper_bound(T k) { return bound(k, false); }
  Node<T>* find(T k) {
    Node<T> *ret = lower_bound(k);
    if (!ret || ret->key != k) {
      return nullptr;
    }
    return ret;
  }
  pair<Node<T>*, bool> insert(T k) {
    Node<T> *node = lower_bound(k);
    if (node && node->key == k) return {node, false};
    root = new Node<T>(k);
    sz++;
    if (!node) {
      if (sz == 1) {
        beg = root; rbeg = root;
        return {root, true};
      }
      node = rbeg;
      node->splay();
      root->l = node;
    } else {
      // now node->key > k
      root->l = node->l; root->r = node;
      node->l = nullptr; node->p = root;
    }
    if (root->l) root->l->p = root;
    if (beg->key > k) beg = root;
    if (rbeg->key < k) rbeg = root;
    return {root, true};
  }
  void erase(Node<T> *node) {
    node->splay();
    if (beg->key == node->key) beg = node->r;
    if (rbeg->key == node->key) rbeg = node->l;
    sz--;
    // now root's key is equal to k
    if (!node->l) {
      root = node->r; delete node;
    } else if (!node->r) {
      root = node->l; delete node;
    } else {
      Node<T> *nx = node->l;
      nx->p = nullptr; // cut left tree
      while (nx->r) nx = nx->r;
      // now nx's key is maximum in root's left tree
      // nx will be root
      nx->splay();
      root = nx;
      // now nx doesn't have right child
      nx->r = node->r;
      delete node;
      nx->r->p = nx;
    }
    if (root) root->p = nullptr;
  }
  bool erase(T k) {
    if (!find(k)) return false;
    erase(root);
    return true;
  }
private:
  Node<T> *root, *beg, *rbeg;
  int sz;
  Node<T>* bound(T k, bool lower) {
    Node<T> *left = root, *right = nullptr;
    while (left) {
      if ((lower && !(k > left->key)) || (!lower && (k < left->key))) {
        right = left;
        left = left->l;
      } else {
        left = left->r;
      }
    }
    if (right) {
      right->splay();
      root = right;
    }
    return right;
  }
};