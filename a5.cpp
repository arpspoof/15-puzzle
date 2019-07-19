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

using namespace std;

char getC(int v) {
	if (v == 8) return ' ';
	else return v + '0';
}

void display(Puzzle& p) {
	auto s = p.state;
	printf("%c %c %c\n%c %c %c\n%c %c %c\n\n",
		getC(s.get(0)), getC(s.get(1)), getC(s.get(2)),
		getC(s.get(3)), getC(s.get(4)), getC(s.get(5)), 
		getC(s.get(6)), getC(s.get(7)), getC(s.get(8))
	);
}

char getC4(int v) {
	if (v == 15) return ' ';
	if (v >= 10) return 'A' + v - 10;
	return v + '0';
}

void display4(Puzzle& p) {
	auto s = p.state;
	printf("%c %c %c %c\n%c %c %c %c\n%c %c %c %c\n%c %c %c %c\n\n",
		getC4(s.get(0)), getC4(s.get(1)), getC4(s.get(2)), getC4(s.get(3)),
		getC4(s.get(4)), getC4(s.get(5)), getC4(s.get(6)), getC4(s.get(7)), 
		getC4(s.get(8)), getC4(s.get(9)), getC4(s.get(10)), getC4(s.get(11)),
		getC4(s.get(12)), getC4(s.get(13)), getC4(s.get(14)), getC4(s.get(15))
	);
}



void computeAndStoreDB(const char *name, DisjointPatternDB &dpd) {
	printf("computing %s ...\n", name);
	dpd.compute();
	printf("computing %s done, writing to file ...\n", name);
	string path = "E:/" + string(name) + ".db";
	dpd.storeToFile(path.c_str());
	printf("writing to file done\n");
}

vector<int> tileGroup1{ 0, 1, 4, 5 };
vector<int> tileGroup2{ 8, 9, 12, 13, 14 };
vector<int> tileGroup3{ 2, 3, 6, 7, 10, 11 };
DisjointPatternDB dpd1r(4, tileGroup1);
DisjointPatternDB dpd2r(4, tileGroup2);
DisjointPatternDB dpd3r(4, tileGroup3);

int pd(Puzzle p) {
	int h1 = dpd1r.getDBValue(p.state);
	int h2 = dpd2r.getDBValue(p.state);
	int h3 = dpd3r.getDBValue(p.state);
	return h1 + h2 + h3;
}

int manhattan(Puzzle p) {
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

int main()
{
	dpd1r.readFromFile("E:/15puzzle-test-db1.db");
	dpd2r.readFromFile("E:/15puzzle-test-db2.db");
	dpd3r.readFromFile("E:/15puzzle-test-db3.db");
	
	for (int i = 0; i < 10000; i++) {
		Puzzle p = getRandomPuzzle(4);
		assert(p.isSolvable());
		display4(p);
		int h1 = dpd1r.getDBValue(p.state);
		int h2 = dpd2r.getDBValue(p.state);
		int h3 = dpd3r.getDBValue(p.state);
		printf("h1=%d,h2=%d,h3=%d\n", h1, h2, h3);
		SearchResult res = IDA(p, manhattan);
		printf("solved problem %d, solution = %d, iter = %d, nodes = %llu, last_nodes = %llu, ms = %lf\n",
			i, res.length, res.nIterations,
			res.nNodesExpanded, res.nNodesInLastInteration, res.runTime);
		char *path = res.path;
		assert(p.verifySolution(path));
	}

/*	for (int i = 0; i < 10000; i++) {
		Puzzle p = getRandomPuzzle(4);
		assert(p.isSolvable());
		display4(p);
		SearchResult res = IDA(p, manhattan);
		printf("solved problem %d, solution = %d, iter = %d, nodes = %llu, last_nodes = %llu, ms = %lf\n",
			i, res.length, res.nIterations,
			res.nNodesExpanded, res.nNodesInLastInteration, res.runTime);
		char *path = res.path;
		assert(p.verifySolution(path));
	}*/

/*	PuzzleState s;
	for (int i = 0; i < 10000; i++) {
		Puzzle p = getRandomPuzzle(3);
		assert(p.isSolvable());
		SearchResult res = IDA(p, manhattan);
		printf("solved problem %d, solution = %d\n", i, res.length);
		char *path = res.path;
		assert(p.verifySolution(path));
	}*/


	/*vector<int> tileGroup1{ 0, 1, 2, 3, 4, 5, 6, 7 };
	DisjointPatternDB dpd1w(4, tileGroup1);
	computeAndStoreDB("15puzzle-db1", dpd1w);*/
	/*vector<int> tileGroup2{ 8, 9, 10, 11, 12, 13, 14 };
	DisjointPatternDB dpd2w(4, tileGroup2);
	computeAndStoreDB("15puzzle-db2", dpd2w);*/

/*	vector<int> tileGroup1{ 0, 1, 4, 5 };
	vector<int> tileGroup2{ 8, 9, 12, 13, 14 };
	vector<int> tileGroup3{ 2, 3, 6, 7, 10, 11 };
	DisjointPatternDB dpd1w(4, tileGroup1);
	DisjointPatternDB dpd2w(4, tileGroup2);
	DisjointPatternDB dpd3w(4, tileGroup3);
	computeAndStoreDB("15puzzle-test-db1", dpd1w);
	computeAndStoreDB("15puzzle-test-db2", dpd2w);
	computeAndStoreDB("15puzzle-test-db3", dpd3w);
	DisjointPatternDB dpd1r(4, tileGroup1);
	DisjointPatternDB dpd2r(4, tileGroup2);
	DisjointPatternDB dpd3r(4, tileGroup3);
	dpd1r.readFromFile("E:/15puzzle-test-db1.db");
	dpd2r.readFromFile("E:/15puzzle-test-db2.db");
	dpd3r.readFromFile("E:/15puzzle-test-db3.db");
	for (int i = 0; i < (int)dpd1r.db.size(); i++) {
		if (dpd1r.db[i] != dpd1w.db[i]) {
			printf("error founded in db1:%d!\n", i);
		}
	}
	for (int i = 0; i < (int)dpd2r.db.size(); i++) {
		if (dpd2r.db[i] != dpd2w.db[i]) {
			printf("error founded in db2:%d!\n", i);
		}
	}
	for (int i = 0; i < (int)dpd3r.db.size(); i++) {
		if (dpd3r.db[i] != dpd3w.db[i]) {
			printf("error founded in db3:%d!\n", i);
		}
	}*/

	/*vector<int> tmp{ 14, 1, 15, 6, 3, 5, 2, 11, 8, 0, 12, 13, 4, 10, 7, 9 };
	PuzzleState s;
	for (int i = 0; i < (int)tmp.size(); i++) {
		s.set(i, tmp[i]);
	}
	int id = dpd.getStateIndex(s);
	int combi = id / factorialMap[8];
	int permu = id % factorialMap[8];

	PermutationIndex pId(8);
	CombinationIndex<4> cId4(16, 8);

	auto bbCombi = cId4.getCombination(combi);
	auto bbPermu = pId.getPermutation(permu);
	
	vector<int> tmpres(16, 0);
	for (int i = 0; i < 8; i++) {
		int ix = bbCombi.get(i);
		tmpres[ix] = tileGroup[bbPermu.get(i)];
	}
	for (int i = 0; i < 16; i++) {
		printf("%d ", tmpres[i]);
	}
	printf("\n");*/

	/*int nums[9] = { 0 };
	int ans = -1;
	int counter = 0;
	ifstream input("E:/teset.csv");
	while (input >> nums[0]) {
		for (int i = 1; i < 9; i++) {
			input >> nums[i];
		}
		input >> ans;
		for (int i = 0; i < 9; i++) {
			if (nums[i] == 0) nums[i] = 8;
			else nums[i]--;
		}
		Puzzle p(3, nums);
		assert(p.isSolvable());
		SearchResult res = IDA(p, manhattan);
		assert(res.length == ans);
		printf("solved problem %d, solution = %d, iter = %d, nodes = %llu, last_nodes = %llu, ms = %lf\n", 
			counter++, res.length, res.nIterations, 
			res.nNodesExpanded, res.nNodesInLastInteration, res.runTime);
	}*/

	system("pause");
	return 0;
}
