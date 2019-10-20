#ifndef GRAPHS_HPP
#define GRAPHS_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <cstdio>
#include "../includes/run.hpp"

template<typename V=Point> class Graph {
public:
	vector<V> vertices;
	vector<vector<int>> edges;

	const inline int size() {
		return (int)this->vertices.size();
	}

	inline int add(const V& vertex) {
		this->vertices.push_back(vertex);
		this->edges.push_back({});
		return (int)this->vertices.size() - 1;
	}

	bool link_exist(int v1, int v2) {
		for (auto& el : edges[v1]) {
			if (el == v2) {
				return true;
			}
		}
		return false;
	}

	inline void link(int v1, int v2) {
		edges[v1].push_back(v2);
		edges[v2].push_back(v1);
	}

	void unlink(int v1, int v2) {
		for (int dir = 0; dir < 2; dir++) {
			for (int iEdge = 0; iEdge < (int)edges[v1].size(); iEdge++) {
				if (edges[v1][iEdge] == v2) {
					if (iEdge != (int)edges[v1].size()-1) {
						swap(edges[v1][iEdge], edges[v1][(int)edges[v1].size()-1]);
					}
					edges[v1].pop_back();
					break;
				}
			}
			swap(v1, v2);
		}
	}
};

struct NavigStats {
	int nbSteps = 0;
	int nbComp = 0;
};


// Global used as temp space
vector<int> nav_last_seen;
vector<double> dist_to_target;
int nav_i_time = 0;
vector<int> in_near_group;
int iQueryOptim = 0;

class NavigableGraph : public Graph<Point> {
public:
	NavigStats nav_stats;

	void resetStats() {
		nav_stats = NavigStats();
	}

	vector<int> getRandPts(int nbPoints) {
		vector<int> ids(nbPoints);
		for (auto& el : ids) {
			el = rand() % vertices.size();
		}
		return ids;
	}

	int stepToNearest(int vertex, const Point& target) {
		int best = vertex;
		double best_dist = dist_sqr(vertices[vertex], target);

		nav_stats.nbComp += edges[vertex].size();
		nav_stats.nbSteps += 1;

		for (int v : edges[vertex]) {
			double d = dist_sqr(vertices[v], target);
			// cerr << d << " / " << best_dist << "\n";
			if (d < best_dist) {
				best_dist = d;
				best = v;
			}
		}
		return best;
	}

	int navigateToNearest(int vertex, const Point& target) {
		while (true) {
			int next = stepToNearest(vertex, target);
			// cerr << next << " vs " << vertex << "\n";
			if (next == vertex) {
				break;
			}
			vertex = next;
		}
		return vertex;
	}

	vector<int> getKnearest(int vertex, const Point& target, int k) {
		if (nav_last_seen.size() < vertices.size()) {
			nav_last_seen.resize(vertices.size(), nav_i_time);
			dist_to_target.resize(vertices.size());
		}
		if (vertices.size() == 0) {
			return {};
		}
		nav_i_time += 1;

		auto comp = [&](const int a, const int b) {
			return dist_to_target[a] < dist_to_target[b];
		};

		queue<int> to_see;
		priority_queue<int, std::vector<int>, decltype(comp)> near_group(comp);
		to_see.push(vertex);
		nav_last_seen[vertex] = nav_i_time;
		dist_to_target[vertex] = dist_sqr(vertices[vertex], target);

		while (!to_see.empty()) {
			int el = to_see.front();
			to_see.pop();
			near_group.push(el);

			if (near_group.size() <= k or near_group.top() != el) {
				nav_stats.nbSteps += 1;
				nav_stats.nbComp += edges[el].size();

				for (int v : edges[el]) {
					if (nav_last_seen[v] != nav_i_time) {
						nav_last_seen[v] = nav_i_time;
						dist_to_target[v] = dist_sqr(vertices[v], target);
						if (near_group.size() < k || !comp(near_group.top(), v)) {
							to_see.push(v);
						}
					}
				}
			}
			while (near_group.size() > k) {
				near_group.pop();
			}
		}

		vector<int> result;
		while (!near_group.empty()) {
			result.push_back(near_group.top());
			near_group.pop();
		}
		reverse(result.begin(), result.end());
		return result;
	}

	vector<int> getKnearestOptim(vector<int> entryPoints, const Point& target, int k,
								int searchLarg, int nbAttemps) {
		if (k >= vertices.size()) {
			vector<int> ans;
			for (int i = 0; i < vertices.size(); i++) {
				ans.push_back(i);
			}
			return ans;
		}
		iQueryOptim += 1;
		if (in_near_group.size() < vertices.size()) {
			in_near_group.resize(vertices.size(), -1);
		}
		if (entryPoints.empty()) {
			entryPoints = this->getRandPts(nbAttemps);
		}
		vector<int> ans;
		auto comp = [&](const int a, const int b) {
			return dist_sqr(vertices[a], target) < dist_sqr(vertices[b], target);
		};
		priority_queue<int, std::vector<int>, decltype(comp)> near_group(comp);

		for (int iAttempt = 0; iAttempt < nbAttemps; iAttempt++) {
			int entryPoint = entryPoints[iAttempt];
			auto ans2 = this->getKnearest(entryPoint, target, searchLarg);
			for (int v : ans2) {
				if (in_near_group[v] != iQueryOptim) {
					in_near_group[v] = iQueryOptim;
					near_group.push(v);
				}
			}
			
			while (near_group.size() > k) {
				near_group.pop();
			}
		}

		while (near_group.size()) {
			ans.push_back(near_group.top());
			near_group.pop();
		}
		return ans;
	}

	void makeInsertion(const Point& vertex, vector<int> vois) {
		int i = this->add(vertex);
		for (int v : vois) {
			this->link(i, v);
		}
	}

	void insert(const Point& vertex, int k, int entryPoint=0) {
		auto vois = getKnearest(entryPoint, vertex, k);
		this->makeInsertion(vertex, vois);
	}

	void insertOptim(const Point& vertex, int k, int searchLarg, int nbAttemps, vector<int> entryPoints={}) {
		auto vois = getKnearestOptim(entryPoints, vertex, k, searchLarg, nbAttemps);
		this->makeInsertion(vertex, vois);
	}
};

#endif