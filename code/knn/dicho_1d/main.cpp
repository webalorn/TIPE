#include "../includes/run.hpp"

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {
	if (nbNeighbours != 1) {
		exit_prog("Only support searching with one neighbour");
	}
	if (nbDim != 1) {
		exit_prog("Only support searching in 1-dim spaces");
	}

	vector<pair<double, int>> pointIds(nbPoints);
	for (int iPoint = 0; iPoint < nbPoints; iPoint++) {
		pointIds[iPoint] = {points[iPoint][0], iPoint};
	}
	sort(pointIds.begin(), pointIds.end());
	stats.buildFinished(nbPoints);

	for (auto target : queries) {
		int searchSteps = 0;
		int a = 0, b = nbPoints;

		while (a+1<b) {
			searchSteps++;
			int m = (a+b-1)/2;
			double d1 = (pointIds[m].first - target[0]); d1 = d1*d1;
			double d2 = (pointIds[m+1].first - target[0]); d2 = d2*d2;

			if (d1 < d2) {
				b = m+1;
			} else {
				a = m+1;
			}
		}

		stats.answer({pointIds[a].second}, searchSteps);
	}
}