#include "index.h"
#include "puzzle.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace std;

long long dbSize;
int combiSize;

vector<unsigned char> result;

PermutationIndex<4> pId(9);
CombinationIndex<4, unsigned long long> cId(16, 9);

static long long nPr(int n, int r) {
	long long result = 1;
	for (int i = n; i > n - r; i--) {
		result *= i;
	}
	return result;
}

void thread()
{
	for (int i = 0; i < 5; ++i)
	{
		std::cout << i << '\n';
	}
}

static void bfs_thread(int workedId) {
	for (int i = 0; i < 1000; i++)
		cout << workedId << endl;
	string inputFile = "dpd_worker_input_" + workedId + string(".tmp.db");
	string outputFile = "dpd_worker_output_" + workedId + string(".tmp.db");
}

void computeDpd(
	int n,
	const string &storePath, 
	const vector<int> &patternTileGroup,
	int nThreads
) {
	int nn = n * n;
	int r = (int)patternTileGroup.size() + 1;
	int emptyTile = nn - 1;

	dbSize = nPr(nn, r);
	combiSize = dbSize / factorialMap[r];

	result = vector<unsigned char>(dbSize, 255);

	vector<int> patternTileMap(nn, emptyTile);
	for (int i = 0; i < (int)patternTileGroup.size(); i++) {
		patternTileMap[patternTileGroup[i]] = i;
	}
	patternTileMap[emptyTile] = r - 1;

	int dummyTile;
	for (int i = 0; i < nn; i++) {
		if (patternTileMap[i] == emptyTile) {
			dummyTile = i;
			break;
		}
	}

	PuzzleState rootCommon;
	for (int i = 0; i < nn; i++) {
		if (patternTileMap[i] == emptyTile || i == emptyTile) {
			rootCommon.set(i, dummyTile);
		}
		else {
			rootCommon.set(i, i);
		}
	}
	
	vector<pair<PuzzleState, unsigned char>> roots;
	for (int i = 0; i < nn; i++) {
		if (patternTileMap[i] == emptyTile || i == emptyTile) {
			PuzzleState copy(rootCommon);
			copy.set(i, emptyTile);
			roots.push_back(make_pair(copy, 0));
		}
	}

	assert(roots.size() == nn - (int)patternTileGroup.size());

	string mergePath = "dpd_merge.tmp.db";

	ofstream rootOutput(mergePath, ios::out | ios::binary);
	for (auto &p : roots) {
		rootOutput << p.first.getStorage() << p.second;
	}
	rootOutput.close();

	ifstream testin(mergePath, ios::in | ios::binary);
	for (int i = 0; i < 8; i++) {
		PuzzleStateStorage s;
		unsigned char v;
		testin >> s >> v;
		printf("%llx, %d\n", s, v);
	}
}
