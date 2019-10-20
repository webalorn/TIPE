#include "../includes/run.hpp"
#include "../includes/graphs.hpp"

const int K_ADD = 16;

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
		int entryPoint = 0; //graph.edges.size() > 1 ? rand() % graph.edges.size() : 0;

		vector<int> ans;
		if (nbNeighbours == 0) {
			int iVertex = graph.navigateToNearest(entryPoint, target);
			ans = {iVertex};
		} else {
			ans = graph.getKnearest(entryPoint, target, nbNeighbours);
		}
		stats.answer(ans, graph.nav_stats.nbSteps - stepsBefore);
	}

	// int N = graph.vertices.size();
	// for (int i = 0; i < N; i++) {
	// 	cerr << i << "\n";
	// }
	// for (int i = 0; i < N; i++) {
	// 	for (int v : graph.edges[i]) {
	// 		cerr << i << " " << v << "\n";
	// 	}
	// }
}