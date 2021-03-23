#include <cassert>
#include <utility>
#include <climits>
#include <random>

struct node {
	int value, cnt, pri;
	bool rev;
	node* lch, * rch;
	node(int v, int p) :value(v), pri(p), cnt(1), rev(false) {
		lch = nullptr;
		rch = nullptr;
	}
};
int count(node* np) { return np ? np->cnt : 0; }
node* update(node* np) {
	np->cnt = count(np->lch) + count(np->rch) + 1;
	return np;
}
void push(node* np) {
	if (np->rev) {
		swap(np->lch, np->rch);
		if (np->lch) np->lch->rev ^= true;
		if (np->rch) np->rch->rev ^= true;
		np->rev = false;
	}
}
int& get(node* np, int pos) {
	assert(0 <= pos && pos <= count(np));
	while (1) {
		push(np);
		if (pos == count(np->lch)) return np->value;
		else if (pos < count(np->lch)) np = np->lch;
		else {
			pos -= count(np->lch) + 1;
			np = np->rch;
		}
	}
}
node* marge(node* npl, node* npr) {
	if (!npl || !npr)return !npl ? npr : npl;
	if (npl->pri > npr->pri) {
		push(npl);
		npl->rch = marge(npl->rch, npr);
		return update(npl);
	}
	else {
		push(npr);
		npr->lch = marge(npl, npr->lch);
		return update(npr);
	}
}
pair<node*, node*>split(node* np, int pos) {
	if (!np) return make_pair(nullptr, nullptr);
	push(np);
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
	node* m = new node(v, engine() & INT_MAX);
	pair<node*, node*>p = split(np, pos);
	return np = marge(marge(p.first, m), p.second);
}
node* erase(node*& np, int pos) {
	assert(pos < count(np));
	pair<node*, node*>p = split(np, pos - 1), p2 = split(p.second, 1);
	delete p2.first;
	return np = marge(p.first, p2.second);
}
node* inversion(node* np, int l, int r) {
	assert(0 <= l && l <= r && r <= count(np));
	if (l == r)return np;
	node* spl[3];
	tie(spl[1], spl[2]) = split(np, r);
	tie(spl[0], spl[1]) = split(spl[1], l);
	spl[1]->rev ^= true;
	return np = marge(spl[0], marge(spl[1], spl[2]));
}
