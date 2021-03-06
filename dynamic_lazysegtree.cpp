#include <vector>
#include <algolithm>
using namespace std;

template <class S,
	S(*op)(S, S),
	S(*e)(),
	class F,
	S(*mapping)(F, S),
	F(*composition)(F, F),
	F(*id)()>
	class dynamic_lazysegtree {
	struct node {
		S sum;
		F lazy;
		node* child[2];
		node() {
			sum = e();
			lazy = id();
			child[0] = nullptr;
			child[1] = nullptr;
		}
		~node() {
			if (child[0])delete child[0];
			if (child[1])delete child[1];
		}
	};
	node* root = new node();
	int size, limit, depth, queryl = 0, queryr = 0;
	S scopy; F fcopy;
	bool (*g)(S);

	inline S getsum(node* np) { return np ? np->sum : e(); }
	inline void eval(node*& np, bool bo) {
		if (np->lazy == id())return;
		if (bo) { //子に伝播
			if (!np->child[0]) np->child[0] = new node();
			np->child[0]->lazy = composition(np->lazy, np->child[0]->lazy);
			if (!np->child[1])np->child[1] = new node();
			np->child[1]->lazy = composition(np->lazy, np->child[1]->lazy);
		}
		np->sum = mapping(np->lazy, np->sum);
		np->lazy = id();
	}
	void set(node*& np, int pos, int dep) {
		if (!np) np = new node();
		eval(np, dep >= 0);
		if (dep == -1) {
			np->sum = scopy;
			return;
		}
		set(np->child[(bool)(pos & (1 << dep))], pos, dep - 1);
		np->sum = op(getsum(np->child[0]), getsum(np->child[1]));
		return;
	}
	void apply(node*& np, int l, int r) {
		if (!np)np = new node();
		eval(np, r - l > 1);
		if (queryl <= l && r <= queryr) {
			np->lazy = composition(fcopy, np->lazy);
			eval(np, r - l > 1);
		}
		else if (queryl < r && l < queryr) {
			apply(np->child[0], l, (l + r) / 2);
			apply(np->child[1], (l + r) / 2, r);
			np->sum = op(getsum(np->child[0]), getsum(np->child[1]));
		}
	}
	S prod(node* np, int l, int r) {
		if (r <= queryl || queryr <= l || !np) {
			return e();
		}
		eval(np, r - l > 1);
		if (queryl <= l && r <= queryr) {
			return np->sum;
		}
		else {
			return op(prod(np->child[0], l, (l + r) / 2), prod(np->child[1], (l + r) / 2, r));
		}
	}
	int max_right(node* np, int l, int r) {
		if (r <= queryl || !np)return -1;
		eval(np, r - l > 1);
		if (g(op(scopy, np->sum))) {
			scopy = op(scopy, np->sum);
			return -1;
		}
		if (r - l == 1) return l;
		int v;
		if ((v = max_right(np->child[0], l, (l + r) / 2)) != -1)return v;
		if ((v = max_right(np->child[1], (l + r) / 2, r)) != -1)return v;
		return -1;
	}
	int min_left(node* np, int l, int r) {
		if (queryr <= l || !np)return -1;
		eval(np, r - l > 1);
		if (g(op(scopy, np->sum))) {
			scopy = op(scopy, np->sum);
			return -1;
		}
		if (r - l == 1)return r;
		int v;
		if ((v = min_left(np->child[1], (l + r) / 2, r)) != -1)return v;
		if ((v = min_left(np->child[0], l, (l + r) / 2)) != -1)return v;
		return -1;
	}
	public:
		dynamic_lazysegtree(int N) {
			assert(0 < N);
			size = N;
			depth = 0;
			while ((1U << depth) < (unsigned int)(N)) depth++;
			limit = 1 << depth;
		}
		dynamic_lazysegtree() {
			depth = 0;
			limit = 1;
		}
		~dynamic_lazysegtree() {
			delete root;
		}
		void set(int pos, S x) {
			assert(0 <= pos && pos < limit);
			scopy = x;
			set(root, pos, depth - 1);
		}
		S get(int pos) {
			assert(0 <= pos && pos < limit);
			node* np = root;
			for (int i = depth - 1; i >= 0; i--) {
				eval(np, true);
				np = np->child[(bool)(pos & (1 << i))];
				if (!np)return e();
			}
			eval(np, false);
			return np->sum;
		}
		void apply(int l, int r, F x) {
			assert(0 <= l && l <= r && r <= limit);
			if (l == r)return;
			queryl = l; queryr = r; fcopy = x;
			apply(root, 0, limit);
		}
		S prod(int l, int r) {
			assert(0 <= l && l <= r && r <= limit);
			if (l == r)return e();
			queryl = l; queryr = r;
			return prod(root, 0, limit);
		}
		template <class Fn> int max_right(int l, Fn temp) {
			assert(0 <= l && l < limit);
			queryl = l;
			scopy = e();
			g = temp;
			int ret = max_right(root, 0, limit);
			if (ret != -1) return ret;
			return size;
		}
		template <class Fn> int min_left(int r, Fn temp) {
			assert(0 < r && r <= limit);
			queryr = r;
			scopy = e();
			g = temp;
			int ret = min_left(root, 0, limit);
			if (ret != -1) return ret;
			return 0;
		}
		S all_prod() {
			eval(root, true);
			return root->sum;
		}
};

/*
int op(int x, int y) { return max(x, y); }
int e() {return 0; }
int mapping(int x, int y) { return x; }
int composition(int after, int now) { return after; }
int id() {return -1; }
*/
