#ifndef RUN_HPP
#define RUN_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

/* Stats about a run on a test */

using Point = vector<double>;
double INF = 1e9;
std::ostream& os = cout;

template<typename T> void read2Dvect(vector<vector<T>>& v) {
	for (auto& l : v) {
		for (auto& el : l) {
			cin >> el;
		}
	}
}

void exit_prog(string error="exited") {
	json output = {
		{"accuracy", .0},
		{"nb_goods", 0},
		{"total", 0},
		{"build_steps", 0},
		{"run_steps", 0},
		{"error", error}
	};
	os << output.dump();
	exit(0);
}

const inline double dist_sqr(const Point& p1, const Point& p2) {
	double s = 0;
	for (int i = 0; i < (int)p1.size(); i++) {
		double d = p1[i]-p2[i];
		s += d*d;
	}
	return s;
}

const inline double dist(const Point& p1, const Point& p2) {
	return sqrt(dist_sqr(p1, p2));
}

class RunStats {
public:
	vector<vector<int>> goodAnswers;
	vector<Point> points;
	vector<Point> queries;
	vector<vector<int>> answers;
	vector<int> answersSteps;
	int nbDim, nbPoints, nbNeighbours, nbQueries;
	int buildSteps = 0;

	void init(int nbDim, int nbPoints, int nbNeighbours, int nbQueries,
		vector<vector<double>> points, vector<Point> queries, vector<vector<int>> goodAnswers) {
		this->goodAnswers = goodAnswers;
		this->points = points;
		this->queries = queries;
		this->nbDim = nbDim;
		this->nbPoints = nbPoints;
		this->nbNeighbours = nbNeighbours;
		this->nbQueries = nbQueries;
	}

	void buildFinished(int nbSteps) {
		this->buildSteps = nbSteps;
	}

	void answer(vector<int> iNearests, int nbSteps) {
		this->answers.push_back(iNearests);
		this->answersSteps.push_back(nbSteps);
	}

	void display() {
		if (answers.size() != goodAnswers.size()) {
			exit_prog("ERROR : not enough answers from the algorithm");
		}

		int nbGoods = 0, total = nbNeighbours * nbQueries;
		for (int iAnswer = 0; iAnswer < this->nbQueries; iAnswer++) {
			sort(answers[iAnswer].begin(), answers[iAnswer].end());

			for (int iPt : answers[iAnswer]) {
				int nbEq = 0;
				for (int iPt2 : answers[iAnswer]) {
					if (iPt == iPt2) {
						nbEq++;
					}
				}
				bool inGood = false;
				for (int iPt2 : goodAnswers[iAnswer]) {
					if (iPt == iPt2) {
						inGood = true;
					}
				}
				if (nbEq < 2 && inGood) {
					nbGoods += 1;
				}
			}

			// if (answers[iAnswer][0] != goodAnswers[iAnswer][0]) {
			// 	cerr << "Q " << iAnswer << " " << answers[iAnswer][0] << "(d=" << dist(points[answers[iAnswer][0]], queries[iAnswer]) << ") instead of " << goodAnswers[iAnswer][0] << "(d=" << dist(points[goodAnswers[iAnswer][0]], queries[iAnswer]) << ")\n";
			// }
		}

		json output = {
			{"accuracy", (double)nbGoods / (double)total},
			{"nb_goods", nbGoods},
			{"total", total},
			{"N", nbPoints},
			{"build_steps", buildSteps},
			{"run_steps", answersSteps}
		};
		os << output.dump();
	}
};

class Prog {
protected:
	RunStats stats;

public:
	void start() {
		ios::sync_with_stdio(false);
		cout.tie(0); cin.tie(0);
		/*
			Input format :
			nb_dim{int}, nb_points{int}, neighbours{int}, nb_queries{int}
			[[coord{double}] * nb_dim] * nb_points
			[[coord{double}] * nb_dim] * nb_queries
			[[answer{int}] * neighbours (sorted)] * nb_queries
		*/

		int nbDim, nbPoints, nbNeighbours, nbQueries;
		cin >> nbDim >> nbPoints >> nbNeighbours >> nbQueries;

		vector<Point> points(nbPoints, Point(nbDim));
		vector<Point> queries(nbQueries, Point(nbDim));;
		vector<vector<int>> goodAnswers(nbQueries, vector<int>(nbNeighbours));;

		read2Dvect(points);
		read2Dvect(queries);
		read2Dvect(goodAnswers);

		this->stats.init(nbDim, nbPoints, nbNeighbours, nbQueries, points, queries, goodAnswers);
		this->run(nbDim, nbPoints, nbNeighbours, nbQueries, points, queries);
		this->stats.display();
	}

protected:
	void run(int, int, int, int, vector<vector<double>>, vector<Point>);
};

int main() { (new Prog)->start(); }

#endif