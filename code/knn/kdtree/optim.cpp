#include "../includes/run.hpp"

using Area = vector<pair<double, double>>;

int buildSteps = 0;
int searchSteps = 0;

struct Vertex {
	Point pt;
	int iPoint;
	int splitAxe;
	Area area;
	pair<int, int> childs;
};

void extend(Area& area, const Area& sub) {
	for (int iDim = 0; iDim < (int)area.size(); iDim++) {
		area[iDim].first = min(area[iDim].first, sub[iDim].first);
		area[iDim].second = max(area[iDim].second, sub[iDim].second);
	}
}

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

	auto& pt = points[ptIds[iCenter]];
	Area area(pt.size());
	for (int i = 0; i < nbDim; i++) {
		area[i] = {pt[i], pt[i]};
	}

	tree.push_back({pt, ptIds[iCenter], iDim, area, {-1, -1}});

	auto idsLeft = vector<int>(ptIds.begin(), ptIds.begin() + iCenter);
	auto idsRight = vector<int>(ptIds.begin() + iCenter + 1, ptIds.end());

	int left = buildTree(points, idsLeft, nbDim, (iDim+1)%nbDim, tree);
	int right = buildTree(points, idsRight, nbDim, (iDim+1)%nbDim, tree);
	tree[iCenterVertex].childs = {left, right};

	if (left != -1) { extend(tree[iCenterVertex].area, tree[left].area); }
	if (right != -1) { extend(tree[iCenterVertex].area, tree[right].area); }

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
	Point nearPossible = target;
	auto& area = tree[iVertex].area;
	for (int iDim = 0; iDim < (int)target.size(); iDim++) {
		nearPossible[iDim] = max(nearPossible[iDim], area[iDim].first);
		nearPossible[iDim] = min(nearPossible[iDim], area[iDim].second);
	}
	if (dist(nearPossible, target) >= bestDist) {
		return {bestDist, bestVertex};
	}

	searchSteps += 1;
	double distVertex = dist(target, tree[iVertex].pt);
	if (distVertex < bestDist) {
		bestDist = distVertex;
		bestVertex = iVertex;
	}

	int childSide = tree[iVertex].childs.first, childOpposite = tree[iVertex].childs.second;

	updateWith(bestDist, bestVertex, searchNearest(tree, target, childSide, bestDist, bestVertex));
	updateWith(bestDist, bestVertex, searchNearest(tree, target, childOpposite, bestDist, bestVertex));

	return {bestDist, bestVertex};
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