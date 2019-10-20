#include "../includes/run.hpp"
#include "../includes/graphs.hpp"

const int K_ADD = 16;
const int REQ_FACT = 2;
const int REQ_MIN = 8;

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {
	NavigableGraph graph;

	for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
		graph.insert(points[iPoint], K_ADD);
	}

	stats.buildFinished(graph.nav_stats.nbSteps);
	graph.resetStats();

	for (auto target : queries) {
		int stepsBefore = graph.nav_stats.nbSteps;

		vector<int> ans;
		ans = graph.getKnearest(0, target, max(REQ_MIN, nbNeighbours*REQ_FACT));
		ans.resize(nbNeighbours);
		stats.answer(ans, graph.nav_stats.nbSteps - stepsBefore);
	}
}