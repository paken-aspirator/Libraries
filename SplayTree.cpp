/*
このコードはhttps://qiita.com/0xdef12e/items/299f67e6a52390c4bcec
を参考にし、一部を改変したものです
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <cstdio>
using namespace std;

template<class T>class SplayTree {
	struct node {
		T val;
		node* lch, * rch;
	};
	node* root = nullptr;
	node* rightRotate(node* np) {
		node* a = np->lch;
		np->lch = a->rch;
		a->rch = np;
		return a;
	}
	node* leftRotate(node* np) {
		node* a = np->rch;
		np->rch = a->lch;
		a->lch = np;
		return a;
	}
	node* splay(node* np, int v) {
		if (!np || np->val == v)return np;
		if (v < np->val) {
			if (!np->lch)return np;
			if (v < np->lch->val) {
				np->lch->lch = splay(np->lch->lch, v);
				np = rightRotate(np);
			}
			else if (np->lch->val < v) {
				np->lch->rch = splay(np->lch->rch, v);
				if (np->lch->rch) np->lch = leftRotate(np->lch);
			}
			return (np->lch) ? rightRotate(np) : np;
		}
		else {
			if (!np->rch)return np;
			if (v < np->rch->val) {
				np->rch->lch = splay(np->rch->lch, v);
				if (np->rch->lch)np->rch = rightRotate(np->rch);
			}
			else if (np->rch->val < v) {
				np->rch->rch = splay(np->rch->rch, v);
				np = leftRotate(np);
			}
			return (np->rch) ? leftRotate(np) : np;
		}
	}
	node* insert(node* np, T v) {
		if (!np) {
			node* q = new node;
			q->val = v;
			q->lch = q->rch = nullptr;
			return q;
		}
		if (v < np->val)np->lch = insert(np->lch, v);
		else np->rch = insert(np->rch, v);
		return np;
	}
	node* erase(node* np, T v) {
		/*if (!np)return nullptr;
		if (v < np->val)np->lch = erase(np->lch, v);
		else */if (np->val < v)np->rch = erase(np->rch, v);
		else if (!np->lch) {
			node* q = np->rch;
			delete np;
			return q;
		}
		else if (!np->lch->rch) {
			node* q = np->lch;
			q->rch = np->rch;
			delete np;
			return q;
		}
		else {
			node* q;
			for (q = np->lch; q->rch->rch; q = q->rch);
			node* r = q->rch;
			q->rch = r->lch;
			r->lch = np->lch;
			r->rch = np->rch;
			delete np;
			return r;
		}
		return np;
	}
public:
	node* find(T x) {
		root = splay(root, x);
		if (!root || root->val != x)return nullptr;
		return root;
	}
	void insert(T x) {
		if (!find(x)) {
			root = insert(root, x);
		}
	}
	void erase(T x) {
		if (find(x)) {
			root = erase(root, x);
		}
	}
};
