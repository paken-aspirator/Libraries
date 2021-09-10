#include <cstdint>
#include <random>
#include <climits>
using namespace std;

std::random_device seed_gen;
class xorshift {
	uint64_t seed;
public:
	xorshift() { seed = (uint64_t(seed_gen()) << 32) + seed_gen(); }
	inline uint64_t get64() {
		seed ^= (seed << 13); seed ^= (seed >> 7);
		return seed ^= (seed << 17);
	}
	// [0, 2^64-1)
	inline uint32_t operator()() { return get64(); }
	// [0, r)
	inline uint32_t operator()(uint32_t r) { return operator()() % r; }
	// [mi, ma)
	inline int operator()(int mi, int ma) { return mi + operator()(ma - mi); }
	// [0,1)
	inline double prob() { return double(operator()()) / 0xffffffff; }
};
