#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
using namespace std;

namespace Library {
	const vector<int> ver = { 1,0,-1,0 }, sid{ 0,1,0,-1 };
	struct grid_on_dfs {
		vector<vector<char>>grid;
		int H, W;
		grid_on_dfs(int xH, int xW) {
			H = xH;
			W = xW;
			grid.resize(xH, vector<char>(xW));
		}
		grid_on_dfs(vector<vector<char>>& xx) {
			H = xx.size();
			W = xx[0].size();
			grid = xx;
		}
		vector<char> operator[](int& other) {
			return grid[other];
		}
		void candidate(vector<pair<int, int>>& next, pair<int, int> p) {//行先の候補をリストにして返す
			

		}
		bool judge(pair<int, int> p) {//行先が行ける場所か判定する

		}
		vector<vector<int>> bfs(pair<int, int> start) {
			int counter = 1;
			vector<vector<int>>flag(H, vector<int>(W, -1));
			flag[start.first][start.second] = 0;
			queue<pair<int, int>>que;
			que.push(start);
			while (!que.empty()) {
				int que_size = que.size();
				for (int i = 0; i < que_size; i++) {
					pair<int, int> p = que.front(); que.pop();
					vector<pair<int, int>> next;
					candidate(next, p);
					for (pair<int, int> p2 : next) {
						if (p2.first >= 0 && p2.first < H &&
							p2.second >= 0 && p2.second < W && flag[p2.first][p2.second] < 0 &&
							judge(p2)) {
							flag[p2.first][p2.second] = counter;
							/*-------処理を書く----*/
							que.push(p2);
							/*---------------------*/
						}
					}
				}
				counter++;
			}
			return flag;
		}
	};
	vector<vector<int>> bfs(pair<int,int> start, vector<vector<char>>&grid) {
		int H = grid.size(), W = grid[0].size(), counter = 1;
		vector<vector<int>>flag(H, vector<int>(W, -1));
		flag[start.first][start.second] = 0;
		queue<pair<int, int>>que;
		que.push(start);
		while (!que.empty()) {
			int que_size = que.size();
			for (int i = 0; i < que_size; i++) {
				pair<int, int> p = que.front(); que.pop();
				for (int j = 0; j < 4; j++) {
					int a = p.first + ver[j], b = p.second + sid[j];
					if (a >= 0 && a < H &&
						b >= 0 && b < W &&
						grid[a][b] == '.' && flag[a][b] < 0) {

						flag[a][b] = counter;
						que.push(make_pair(a, b));
					}
				}
			}
			counter++;
		}
		return flag;
	}
}
