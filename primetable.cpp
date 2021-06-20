#include<cassert>
using namespace std;

class primetable {
	int size;
	bool* table;
public:
	primetable(int N) {
		size = N;
		table = new bool[size];
		for (int i = 0; i < size; i++)table[i] = true;
		if (size > 0)table[0] = false;
		if (size > 1)table[1] = false;
		for (int i = 2; i < size; i++) {
			if (!table[i])continue;
			for (int j = i * 2; j < size; j += i) {
				table[j] = false;
			}
		}
	}
	bool operator()(int num) {
		assert(0 <= num && num < size);
		return table[num];
	}
};
