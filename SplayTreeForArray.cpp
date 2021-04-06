#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

template<class T>struct SplayTreeForArray {
	struct node {
		T val;
		int cnt;
		node* lch, * rch;
		node(T v) {
			val = v;
			cnt = 1;
			lch = nullptr;
			rch = nullptr;
		}
	};
	int count(node* np) { return np ? np->cnt : 0; }
	node* update(node* np) { np->cnt = count(np->lch) + count(np->rch) + 1; return np; }
	node* root = nullptr;
private:
	node* rightRotate(node* np) {
		node* a = np->lch;
		np->lch = a->rch;
		a->rch = np;
		update(np);
		return update(a);
	}
	node* leftRotate(node* np) {
		node* a = np->rch;
		np->rch = a->lch;
		a->lch = np;
		update(np);
		return update(a);
	}
	node* erase_sub(node* np) {//rootの削除
		if (!np)return nullptr;
		if (!np->lch) {
			node* q = np->rch;
			delete np;
			return q;
		}
		else if (!np->lch->rch) {
			node* q = np->lch;
			q->rch = np->rch;
			delete np;
			return update(q);
		}
		else {
			node* q;
			for (q = np->lch; q->rch->rch; q = q->rch)q->cnt--;
			q->cnt--;
			node* r = q->rch;
			q->rch = r->lch;
			r->lch = np->lch;
			r->rch = np->rch;
			delete np;
			update(q);
			return update(r);
		}
	}
	node* splay(node* np, int pos) {//pos番目のノードをrootに
		if (!np || count(np->lch) == pos)return np;
		if (pos < count(np->lch)) {
			if (!np->lch)return np;
			if (pos < count(np->lch->lch)) {
				np->lch->lch = splay(np->lch->lch, pos);
				np = rightRotate(np);
			}
			else if (count(np->lch->lch) < pos) {
				np->lch->rch = splay(np->lch->rch, pos - count(np->lch->lch) - 1);
				if (np->lch->rch) np->lch = leftRotate(np->lch);
			}
			return (np->lch) ? rightRotate(np) : np;
		}
		else {
			pos -= count(np->lch) + 1;
			if (!np->rch)return np;
			if (pos < count(np->rch->lch)) {
				np->rch->lch = splay(np->rch->lch, pos);
				if (np->rch->lch)np->rch = rightRotate(np->rch);
			}
			else if (count(np->rch->lch) < pos) {
				np->rch->rch = splay(np->rch->rch, pos - count(np->rch->lch) - 1);
				np = leftRotate(np);
			}
			return (np->rch) ? leftRotate(np) : np;
		}
	}
public:
	T get(node*np, int pos) {
		splay(np, pos);
		assert(np);
		return np->val;
	}
	node* insert(node*& np, int pos, T v) {//node* np のpos番目に要素がvの要素を挿入
		if (!np) {
			node* q = new node(v);
			return np=q;
		}
		if (count(np->lch) < pos)np->rch = insert(np->rch, pos - count(np->lch) - 1, v);
		else np->lch = insert(np->lch, pos, v);
		return update(np);
	}
	void erase(node*& np,int pos) {
		np = splay(np, pos);
		np = erase_sub(np);
		return;
	}
	pair<node*, node*>split(node* np, int pos) { //[0, pos), [pos, n)
		insert(np, pos, T());
		np = splay(np, pos);
		pair<node*, node*>p = { np->lch,np->rch };
		delete np;
		return p;
	}
	node* merge(node* l, node* r) {
		node* np = new node(T());
		np->lch = l;
		np->rch = r;
		return erase_sub(np);
	}
	void print(node* np) {
		if (!np)return;
		print(np->lch);
		cout << np->val << " ";
		print(np->rch);
	}
};
