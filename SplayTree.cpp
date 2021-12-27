#include <bits/stdc++.h>
using namespace std;

template <class T> struct Node {
  Node<T> *l, *r, *p;
  T key;
  Node() : l(nullptr), r(nullptr), p(nullptr) {}
  int state() {
    if(p && p->l == this) return -1;
    if(p && p->r == this) return 1;
    return 0;
  }
  void rotate() {
    Node<T> *par = p;
    Node<T> *mid;
    if(p->l == this) {
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
      if(st == 0) {
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
  SplayTree() : root(nullptr), top(nullptr), sz(0) {}
  int size() { return sz; }
  Node<T> *begin() { return top; }
  Node<T>* end() { return nullptr; }
  Node<T>* find(T k) {
    Node<T> *ret = search(k);
    if (!ret || ret->key != k) {
      return nullptr;
    }
    return ret;
  }
  Node<T>* lower_bound(T k) {
    Node<T> *ret = search(k);
    if (!ret) return ret;
    if (ret->key < k) ret = ret->next();
    return ret;
  }
  Node<T>* upper_bound(T k) {
    Node<T> *ret = search(k);
    if (!ret) return ret;
    if (ret->key <= k) ret = ret->next();
    return ret;
  }
  pair<Node<T>*, bool> insert(T k) {
    Node<T> *node = search(k);
    if (node && node->key == k) return {node, false};
    root = new Node<T>;
    root->key = k; sz++;
    if (!top || top->key > k) top = root;
    if (!node) return {root, true};
    Node<T> *tmp;
    if (node->key > k) {
      tmp = node->l; node->l = nullptr;
      root->l = tmp; root->r = node;
    } else if (node->key < k) {
      tmp = node->r; node->r = nullptr;
      root->l = node; root->r = tmp;
    }
    node->p = root;
    if (tmp) tmp->p = root;
    return {root, true};
  }
  void erase(Node<T> *node) {
    node->splay();
    if (top->key == node->key) top = node->r;
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
  Node<T> *root, *top;
  int sz;
  Node<T>* search(T k) {
    if (!root) return nullptr;
    Node<T> *node = root;
    while (1) {
      if (node->key == k) break;
      if (node->key > k) {
        if (!node->l) break;
        node = node->l;
      } else {
        if (!node->r) break;
        node = node->r;
      }
    }
    node->splay();
    root = node;
    return root;
  }
};