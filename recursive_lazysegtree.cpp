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
	class lazy_segtree {
	int size, limit, depth, queryl, queryr;
	std::vector<S>data;
	std::vector<F>lazy;
	void update(int k) { data[k] = op(data[2 * k], data[2 * k + 1]); }
	void eval(int k) {
		if (lazy[k] == id())return;
		if (k < limit) {
			composition(lazy[k], lazy[k * 2]);
			composition(lazy[k], lazy[k * 2 + 1]);
		}
		data[k] = mapping(lazy[k], data[k]);
		lazy[k] = id();
	}
	void apply(F x, int k, int l, int r) {
		eval(k);
		if (queryl <= l && r <= queryr) {
			composition(x, lazy[k]);
			eval(k);//これをしないと5行後詰む
		}
		else if (queryl < r && l < queryr) {
			apply(x, k * 2, l, (l + r) / 2);
			apply(x, k * 2 + 1, (l + r) / 2, r);
			data[k] = op(data[k * 2], data[k * 2 + 1]);
		}
	}
	S query(int k, int l, int r) {
		eval(k);
		if (r <= queryl || queryr <= l) {
			return e();
		}
		else if (queryl <= l && r <= queryr) {
			return data[k];
		}
		else {
			return op(query(k * 2, l, (l + r) / 2), query(k * 2 + 1, (l + r) / 2, r));
		}
	}
	public:
		lazy_segtree(int N) :size(N) {
			assert(0 <= N);
			depth = 0;
			while ((1 << depth) < size)depth++;
			data.resize(limit * 2, e());
			lazy.resize(limit * 2, id());
		}
		lazy_segtree(const std::vector<S>& v) :size((int)v.size()) {
			limit = 1;
			while (limit < size)limit <<= 1;
			data.resize(limit * 2, e());
			lazy.resize(limit * 2, id());
			for (int i = 0; i < size; i++) data[i + limit] = v[i];
			for (int i = limit - 1; i > 0; i--) update(i);
		}
		void set(int pos, S x) {
			assert(0 <= pos && pos < size);
			pos += limit;
			for (int i = depth; i >= 0; depth--)eval(pos >> i);
			data[pos] = x;
			for (pos >>= 1; pos > 0; pos >>= 1) update(pos);
		}
		S get(int pos) {
			assert(0 <= pos && pos < size);
			pos += limit;
			for (int i = depth; i >= 0; depth--)eval(pos >> i);
			return data[pos];
		}
		void apply(int a,int b,F x) {
			assert(0 <= a && a <= b && b < size);
			if (a == b)return;
			queryl = a; queryr = b;
			apply(x, 1, 0, limit);
		}
		S query(int a, int b) {
			assert(0 <= a && a <= b && b < size);
			if (a == b)return e();
			queryl = a; queryr = b;
			return query(1, 0, limit);
		}
};
