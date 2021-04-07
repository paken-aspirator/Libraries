#include <cstdint>
#include <random>
#include <climits>
using namespace std;

std::random_device seed_gen;
class xorshift {
	uint32_t seed;
public:
	xorshift() { seed = seed_gen(); }
	inline uint64_t get64() {
		seed ^= (seed << 13); seed ^= (seed >> 7);
		return seed ^= (seed << 17);
	}
	inline uint32_t get() { return get64(); }
	inline uint32_t operator()() { return get64(); }
	inline uint32_t get(uint32_t r) { return operator()() % r; }
	inline int get(int mi, int ma) { return mi + get(ma - mi); }
	inline double prob() { return double(operator()()) / 0xffffffff; }
};
