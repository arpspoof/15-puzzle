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

int manhattan(Puzzle p) {
	int value = 0;
	for (int i = 0; i < 9; i++) {
		int element = p.state.get(i);
		if (element == 8) {
			continue;
		}
		int r1 = i / 3, c1 = i % 3;
		int r2 = element / 3, c2 = element % 3;
		value += abs(r1 - r2) + abs(c1 - c2);
	}
	return value;
}

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

void computeAndStoreDB(const char *name, DisjointPatternDB &dpd) {
	printf("computing %s ...\n", name);
	dpd.compute();
	printf("computing %s done, writing to file ...\n", name);
	string path = "E:/" + string(name) + ".db";
	dpd.storeToFile(path.c_str());
	printf("writing to file done\n");
}

int main()
{
/*	PuzzleState s;
	for (int i = 0; i < 10000; i++) {
		Puzzle p = getRandomPuzzle(3);
		assert(p.isSolvable());
		SearchResult res = IDA(p, manhattan);
		printf("solved problem %d, solution = %d\n", i, res.length);
		char *path = res.path;
		assert(p.verifySolution(path));
	}*/

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
	printf("\n");

	int nums[9] = { 0 };
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
