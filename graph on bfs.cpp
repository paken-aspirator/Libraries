#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

namespace Library {
	vector<int> graph_on_bfs(vector<vector<int>>& graph, int start) {
		assert(0<=start&&start<graph.size());
		vector<int>answer(graph.size(), INF);
		answer[start] = 0;
		int front;
		queue<int>que;
		que.push(start);
		while (!que.empty()) {
			front = que.front(); que.pop();
			for (int& t : graph[front]) {
				if (answer[t] == INF) {
					answer[t] = answer[front] + 1;
					que.push(t);
				}
			}
		}
		return answer;
	}
}
