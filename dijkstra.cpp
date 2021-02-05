#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <queue>
#include <cassert>
using namespace std;

constexpr int INF = 1070000000;
constexpr long long LINF = 4610000000000000000;

struct edge {
	int to, cost;
};

template<class T>bool chmin(T& xx, T yy) { if (xx > yy) { xx = yy; return true; }return false; }

namespace Library {
	template<class T>vector<T>dijkstra(vector<vector<edge>>& graph, int start) {//到達不可能な時はINFを返す
		assert(0 <= start && start < graph.size());
		priority_queue<pair<int, T>, vector<pair<int, T>>, greater<pair<int, T>>>que;
		que.push({ start,0 });
		vector<T>dist(graph.size(), (sizeof(T) <= 4 ? 1070000000 : 4610000000000000000ll));
		dist[start] = 0;
		while (!que.empty()) {
			pair<int, T>front = que.top(); que.pop();
			if (front.second > dist[front.first])continue;
			for (edge& p : graph[front.first]) {
				if (chmin(dist[p.to], front.second + p.cost)) {
					que.push(make_pair(p.to, dist[p.to]));
				}
			}
		}
		return dist;
	}
}
