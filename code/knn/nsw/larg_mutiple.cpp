#include "../includes/run.hpp"
#include "../includes/graphs.hpp"
#include <cstdio>

const int K_INSERTION = 8;
const int REQ_FACT = 2; // 1-2
const int REQ_MIN = 2; // 1-8
const int N_ATTEMPT = 3;

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {
	srand(42);
	NavigableGraph graph;

	for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
		// graph.insert(points[iPoint], K_INSERTION);
		graph.insertOptim(points[iPoint], K_INSERTION, K_INSERTION*2, 5);
	}

	stats.buildFinished(graph.nav_stats.nbSteps);
	graph.resetStats();

	for (auto target : queries) {
		int stepsBefore = graph.nav_stats.nbSteps;
		auto ans = graph.getKnearestOptim({}, target, nbNeighbours, max(REQ_MIN, nbNeighbours*REQ_FACT), N_ATTEMPT);
		stats.answer(ans, graph.nav_stats.nbSteps - stepsBefore);
	}
}