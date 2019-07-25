// a5.cpp : Defines the entry point for the application.
//

#include "a5.h"
#include "puzzle.h"
#include "solver.h"
#include "bit_blocks.h"
#include "index.h"
#include "dpd.h"

#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

static char getC4(int v) {
	if (v == 15) return ' ';
	if (v >= 10) return 'A' + v - 10;
	return v + '0';
}

static void display4(Puzzle& p) {
	auto s = p.state;
	printf("%c %c %c %c\n%c %c %c %c\n%c %c %c %c\n%c %c %c %c\n\n",
		getC4(s.get(0)), getC4(s.get(1)), getC4(s.get(2)), getC4(s.get(3)),
		getC4(s.get(4)), getC4(s.get(5)), getC4(s.get(6)), getC4(s.get(7)), 
		getC4(s.get(8)), getC4(s.get(9)), getC4(s.get(10)), getC4(s.get(11)),
		getC4(s.get(12)), getC4(s.get(13)), getC4(s.get(14)), getC4(s.get(15))
	);
}

static void computeAndStoreDB(const char *name, DisjointPatternDB &dpd) {
	printf("computing %s ...\n", name);
	dpd.compute(12, 8192);
	printf("computing %s done, writing to file ...\n", name);
	string path = string(name) + ".db";
	dpd.storeToFile(path.c_str());
	printf("writing to file done\n");
}

static void computePatternDBs() {
	vector<int> tileGroup1{ 0, 1, 2, 3, 4, 5, 6, 7 };
	DisjointPatternDB dpd1w(4, tileGroup1);
	computeAndStoreDB("15puzzle-db1", dpd1w);
	vector<int> tileGroup2{ 8, 9, 10, 11, 12, 13, 14 };
	DisjointPatternDB dpd2w(4, tileGroup2);
	computeAndStoreDB("15puzzle-db2", dpd2w);
}

static vector<int> tileGroup1H4{ 0, 1, 2, 3, 4, 5, 6, 7 };
static vector<int> tileGroup2H4{ 8, 9, 10, 11, 12, 13, 14 };
static DisjointPatternDB dpd1r(4, tileGroup1H4);
static DisjointPatternDB dpd2r(4, tileGroup2H4);

static int dpdh4(Puzzle p) {
	int h1 = dpd1r.getDBValue(p.state);
	int h2 = dpd2r.getDBValue(p.state);
	return h1 + h2;
}

static int manhattan(Puzzle p) {
	int value = 0;
	int n = p.n;
	int nn = p.n * p.n;
	for (int i = 0; i < nn; i++) {
		int element = p.state.get(i);
		if (element == nn - 1) {
			continue;
		}
		int r1 = i / n, c1 = i % n;
		int r2 = element / n, c2 = element % n;
		value += abs(r1 - r2) + abs(c1 - c2);
	}
	return value;
}

int main(int argc, char **argv)
{
	if (argc <= 1) {
		printf("you must specify an argument!\n");
		exit(2);
	}

	if (argv[1] == string("-compute")) {
		computePatternDBs();
		return 0;
	}

	ofstream o(argv[1]);
	o << "puzzle,method,solution-length,solution,initial-h,run-time(ms),nodes-expanded,nodes-per-second,iterations" << endl;
	o.flush();

	dpd1r.readFromFile("15puzzle-db1.db");
	dpd2r.readFromFile("15puzzle-db2.db");

	int nRepeat = 100;
	if (argc > 2) {
		stringstream ss(argv[2]);
		ss >> nRepeat;
	}
	
	for (int i = 0; i < nRepeat; i++) {
		Puzzle p = getRandomPuzzle(4);
		display4(p);

		SearchResult res2 = IDA(p, dpdh4);
		double nodesPerSecond2 = res2.nNodesExpanded * 1000.0 / res2.runTime;
		o << p.toString() << ",disjoint-database," << res2.length << "," << string(res2.path) << ","
			<< dpdh4(p) << "," << res2.runTime << "," << res2.nNodesExpanded << "," <<
			nodesPerSecond2 << "," << res2.nIterations << endl;
		o.flush();
		printf("(disjoint database) solved problem %d, solution = %d, iter = %d, nodes = %llu, nodesPerSecond = %lf, ms = %lf\n",
			i, res2.length, res2.nIterations,
			res2.nNodesExpanded, nodesPerSecond2, res2.runTime);
		assert(p.verifySolution(res2.path));

		SearchResult res1 = IDA(p, manhattan);
		double nodesPerSecond1 = res1.nNodesExpanded * 1000.0 / res1.runTime;
		o << p.toString() << ",manhattan," << res1.length << "," << string(res1.path) << "," 
			<< manhattan(p) << "," << res1.runTime << "," << res1.nNodesExpanded << "," <<
			nodesPerSecond1 << "," << res1.nIterations << endl;
		o.flush();
		printf("(manhattan) solved problem %d, solution = %d, iter = %d, nodes = %llu, nodesPerSecond = %lf, ms = %lf\n",
			i, res1.length, res1.nIterations,
			res1.nNodesExpanded, nodesPerSecond1, res1.runTime);
		assert(p.verifySolution(res1.path));

		if (res1.length != res2.length) {
			printf("there's an error!\n");
			exit(1);
		}
	}

	o.close();

	return 0;
}
