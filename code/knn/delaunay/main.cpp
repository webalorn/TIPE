#include "../includes/run.hpp"
#include "../includes/graphs.hpp"

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {
	if (nbNeighbours != 1) {
		exit_prog("Only support searching with one neighbour");
	}

	// Kdtree tree;
	// tree.reserve(nbPoints);
	// vector<int> pointIds(nbPoints);
	// for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
	// 	pointIds[iPoint] = iPoint;
	// }

	// int root = buildTree(points, pointIds, nbDim, 0, tree);
	// stats.buildFinished(buildSteps);

	// for (auto target : queries) {
	// 	searchSteps = 0;
	// 	int iVertex = searchNearest(tree, target, root, INF, -1).second;
	// 	stats.answer({tree[iVertex].iPoint}, searchSteps);
	// }
}