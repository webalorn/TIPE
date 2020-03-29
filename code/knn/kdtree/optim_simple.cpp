#include "../includes/run.hpp"

int buildSteps = 0;
int searchSteps = 0;

int INT_INF = 1000*1000*1000;

struct Vertex {
	Point pt;
	int iPoint;
	int splitAxe;
	pair<int, int> childs;
	int left, right;
};

using Kdtree = vector<Vertex>;

int buildTree(vector<Point>& points, vector<int> ptIds, int nbDim, int iDim, Kdtree& tree) {
	if (ptIds.empty()) {
		return -1;
	}

	buildSteps += ptIds.size();
	sort(ptIds.begin(), ptIds.end(), [&](int a, int b) {
		return points[a][iDim] < points[b][iDim];
	});
	int iCenter = ptIds.size() / 2;
	int iCenterVertex = tree.size();
	int left_x = (iCenter == 0) ? -INT_INF : points[ptIds[iCenter-1]][iDim];
	int right_x = (iCenter == ptIds.size()-1) ? +INT_INF : points[ptIds[iCenter+1]][iDim];
	tree.push_back({points[ptIds[iCenter]], ptIds[iCenter], iDim, {-1, -1}, left_x, right_x});

	auto idsLeft = vector<int>(ptIds.begin(), ptIds.begin() + iCenter);
	auto idsRight = vector<int>(ptIds.begin() + iCenter + 1, ptIds.end());

	int left = buildTree(points, idsLeft, nbDim, (iDim+1)%nbDim, tree);
	int right = buildTree(points, idsRight, nbDim, (iDim+1)%nbDim, tree);
	tree[iCenterVertex].childs = {left, right};

	return iCenterVertex;
}

void updateWith(double& bestDist, int& bestVertex, pair<double, int> r) {
	bestDist = r.first;
	bestVertex = r.second;
}

pair<double, int> searchNearest(Kdtree& tree, Point& target, int iVertex, double bestDist, int bestVertex) {
	if (iVertex == -1) {
		return {bestDist, bestVertex};
	}
	searchSteps += 1;
	double distVertex = dist(target, tree[iVertex].pt);
	if (distVertex < bestDist) {
		bestDist = distVertex;
		bestVertex = iVertex;
	}

	int childSide = tree[iVertex].childs.first, childOpposite = tree[iVertex].childs.second;
	int left = tree[iVertex].left, right = tree[iVertex].right;
	int axe = tree[iVertex].splitAxe;
	double plan = tree[iVertex].pt[axe];

	if (target[axe] > plan) {
		swap(childSide, childOpposite);
		swap(left, right);
	}

	updateWith(bestDist, bestVertex, searchNearest(tree, target, childSide, bestDist, bestVertex));
	if (abs(right - target[axe]) < bestDist) {
		updateWith(bestDist, bestVertex, searchNearest(tree, target, childOpposite, bestDist, bestVertex));
	}

	return {bestDist, bestVertex};
}

int simpleHeuristicSearch(Kdtree& tree, Point& target, int iVertex) {
	if (iVertex == -1) {
		return -1;
	}
	searchSteps += 1;

	int axe = tree[iVertex].splitAxe;
	double plan = tree[iVertex].pt[axe];
	int childSide = target[axe] <= plan ? tree[iVertex].childs.first : tree[iVertex].childs.second;

	int best = simpleHeuristicSearch(tree, target, childSide);
	if (best == -1) {
		return iVertex;
	}
	double distVertex = dist(target, tree[iVertex].pt);
	double distBest = dist(target, tree[best].pt);

	return distBest < distVertex ? best : iVertex;
}

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {
	if (nbNeighbours != 1) {
		exit_prog("Only support searching with one neighbour");
	}

	Kdtree tree;
	tree.reserve(nbPoints);
	vector<int> pointIds(nbPoints);
	for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
		pointIds[iPoint] = iPoint;
	}

	int root = buildTree(points, pointIds, nbDim, 0, tree);
	stats.buildFinished(buildSteps);

	for (auto target : queries) {
		searchSteps = 0;
		int iVertex = searchNearest(tree, target, root, INF, -1).second;
		stats.answer({tree[iVertex].iPoint}, searchSteps);
	}
}