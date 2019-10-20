#include "../includes/run.hpp"
#include <queue>

void Prog::run(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
	vector<vector<double>> points, vector<Point> queries) {
	stats.buildFinished(0);

	for (auto pt : queries) {
		if (nbNeighbours == 1) {
			int bestId = -1;
			double bestDist = INF * INF;
			for (int iPt = 0; iPt < (int)points.size(); iPt++) {
				double d = dist_sqr(points[iPt], pt);
				if (d < bestDist) {
					bestDist = d;
					bestId = iPt;
				}
			}
			stats.answer({bestId}, (int)points.size());
		} else {
			priority_queue<pair<double, int>> dists;

			for (int iPt = 0; iPt < (int)points.size(); iPt++) {
				dists.push({dist_sqr(points[iPt], pt), iPt});
				if (dists.size() > nbNeighbours) {
					dists.pop();
				}
			}
			vector<int> ans(nbNeighbours);
			for (int iNeighbour = 0; iNeighbour < nbNeighbours; iNeighbour++) {
				ans[iNeighbour] = dists.top().second;
				dists.pop();
			}
			stats.answer(ans, (int)points.size());
		}
	}
}