#include <algorithm>
#include <climits>
#include <random>
#include <utility>
using namespace std;

std::random_device seed_generate;
std::mt19937 rand_gen(seed_generate());

struct node {
	int value, cnt, pri;
	node* lch, * rch;
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
node* marge(node* npl, node* npr) {
	if (!npl || !npr)return !npl ? npr : npl;
	if (npl->pri > npr->pri) {
		npl->rch = marge(npl->rch, npr);
		return update(npl);
	}
	else {
		npr->lch = marge(npl, npr->lch);
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
	return np = marge(marge(p.first, m), p.second);
}
node* erase(node*& np, int pos) {
	assert(pos < count(np));
	pair<node*, node*>p = split(np, pos - 1);
	return np = marge(p.first, split(p.second, 1).second);
}
