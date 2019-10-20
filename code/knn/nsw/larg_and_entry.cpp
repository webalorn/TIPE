#include "../includes/run.hpp"
#include "../includes/graphs.hpp"
#include "../kdtree/kdtree.hpp"

const int K_ADD = 16;
const int REQ_FACT = 2;
const int REQ_MIN = 8;

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {

	// Buiild NSW graph
	NavigableGraph graph;

	for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
		graph.insert(points[iPoint], K_ADD);
	}

	// Build KD-tree to find a good entry point

	Kdtree tree;
	tree.reserve(nbPoints);
	vector<int> pointIds(nbPoints);
	for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
		pointIds[iPoint] = iPoint;
	}

	int root = buildTree(points, pointIds, nbDim, 0, tree);

	stats.buildFinished(graph.nav_stats.nbSteps + buildSteps);
	graph.resetStats();

	for (auto target : queries) {
		int stepsBefore = graph.nav_stats.nbSteps;
		searchSteps = 0; // KD-tree
		int entryPoint = simpleHeuristicSearch(tree, target, root);

		vector<int> ans;
		ans = graph.getKnearest(entryPoint, target, max(REQ_MIN, nbNeighbours*REQ_FACT));
		ans.resize(nbNeighbours);
		stats.answer(ans, graph.nav_stats.nbSteps - stepsBefore + searchSteps);
	}
}