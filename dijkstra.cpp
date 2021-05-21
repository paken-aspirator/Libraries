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
	template<class T, T INF>vector<T>dijkstra(vector<vector<edge>>& graph, int start) {//到達不可能な時はINFを返す
		assert(0 <= start && start < graph.size());
		priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>>que;
		que.push(make_pair(0, start));
		vector<T>dist(graph.size(), INF);
		dist[start] = 0;
		while (!que.empty()) {
			pair<T, int>p = que.top();
			que.pop();
			if (dist[p.second] < p.first)continue;
			for(edge& e : graph[p.second]) {
				if (chmin(dist[e.to], p.first + e.cost)) {
					que.push(make_pair(dist[e.to], e.to));
				}
			}
		}
		return dist;
	}
}
