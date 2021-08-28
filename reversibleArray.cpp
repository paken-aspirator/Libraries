#include <algorithm>
#include <iostream>
#include <climits>
#include <random>
#include <utility>
#include <tuple>
#include <cassert>
using namespace std;

std::random_device seed_generate;
std::mt19937 rand_gen(seed_generate());

namespace treap {
	struct node {
		int value, cnt, pri;
		node* lch, * rch;
		node() {
			lch = nullptr;
			rch = nullptr;
		}
		node(int v, int p) :value(v), pri(p), cnt(1) {
			lch = nullptr;
			rch = nullptr;
		}
	};
	int count(node* np) { return np ? np->cnt : 0; }
	node* update(node* np) {
		np->cnt = count(np->lch) + count(np->rch) + 1;
		return np;
	}
	int& get(node* np, int pos) {
		assert(0 <= pos && pos < count(np));
		while (1) {
			if (pos == count(np->lch)) return np->value;
			else if (pos < count(np->lch)) np = np->lch;
			else {
				pos -= count(np->lch) + 1;
				np = np->rch;
			}
		}
	}
	node* merge(node* npl, node* npr) {
		if (!npl || !npr)return !npl ? npr : npl;
		if (npl->pri > npr->pri) {
			npl->rch = merge(npl->rch, npr);
			return update(npl);
		}
		else {
			npr->lch = merge(npl, npr->lch);
			return update(npr);
		}
	}
	pair<node*, node*>split(node* np, int pos) {
		if (!np) return make_pair(nullptr, nullptr);
		if (pos <= count(np->lch)) {
			pair<node*, node*>p = split(np->lch, pos);
			np->lch = p.second;
			return make_pair(p.first, update(np));
		}
		else {
			pair<node*, node*>p = split(np->rch, pos - count(np->lch) - 1);
			np->rch = p.first;
			return make_pair(update(np), p.second);
		}
	}
	node* insert(node*& np, int pos, int v) {
		assert(0 <= pos && pos <= count(np));
		node* m = new node(v, rand_gen() & INT_MAX);
		pair<node*, node*>p = split(np, pos);
		return np = merge(merge(p.first, m), p.second);
	}
	node* erase(node*& np, int pos) {
		assert(0 <= pos && pos < count(np));
		pair<node*, node*>p = split(np, pos);
		return np = merge(p.first, split(p.second, 1).second);
	}
	void print(node* np) {
		if (!np)return;
		print(np->lch);
		cout << np->value << " ";
		print(np->rch);
	}
}


class reversible_array {
	int size = 0;
	treap::node* tre = nullptr, * rev = nullptr;
	treap::node* splited_tre[3], * splited_rev[3];
public:;
	  reversible_array() {
		  size = 0;
	  }
	  reversible_array(vector<int>v) {
		  size = v.size();
		  for (int i = 0; i < v.size(); i++) {
			  treap::insert(tre, i, v[i]);
			  treap::insert(rev, 0, v[i]);
		  }
	  }
	  int operator[](int index) {
		  return treap::get(tre, index);
	  }
	  void insert(int pos, int v) {
		  treap::insert(tre, pos, v);
		  treap::insert(rev, size - pos, v);
		  size++;
	  }
	  void erase(int pos) {
		  treap::erase(tre, pos);
		  treap::erase(rev, size - pos - 1);
		  size--;
	  }
	  void reverse(int l, int r) {
		  tie(splited_tre[1], splited_tre[2]) = treap::split(tre, r);
		  tie(splited_tre[0], splited_tre[1]) = treap::split(splited_tre[1], l);
		  tie(splited_rev[1], splited_rev[2]) = treap::split(rev, size - l);
		  tie(splited_rev[0], splited_rev[1]) = treap::split(splited_rev[1], size - r);
		  tre = treap::merge(splited_tre[0], treap::merge(splited_rev[1], splited_tre[2]));
		  rev = treap::merge(splited_rev[0], treap::merge(splited_tre[1], splited_rev[2]));
	  }
};
