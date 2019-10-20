int buildSteps = 0;
int searchSteps = 0;

struct Vertex {
	Point pt;
	int iPoint;
	int splitAxe;
	pair<int, int> childs;
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
	tree.push_back({points[ptIds[iCenter]], ptIds[iCenter], iDim, {-1, -1}});

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
	int axe = tree[iVertex].splitAxe;
	double plan = tree[iVertex].pt[axe];

	if (target[axe] > plan) {
		swap(childSide, childOpposite);
	}

	updateWith(bestDist, bestVertex, searchNearest(tree, target, childSide, bestDist, bestVertex));
	if (abs(plan - target[axe]) < bestDist) {
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