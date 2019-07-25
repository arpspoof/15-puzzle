#include "dpd.h"

#include <queue>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>

using namespace std;

static unsigned char DB_UNDEFINED = 255;

static int nPr(int n, int r) {
	int result = 1;
	for (int i = n; i > n - r; i--) {
		result *= i;
	}
	return result;
}

DisjointPatternDB::DisjointPatternDB(int n, const vector<int> &tileGroup)
	:n(n), r((int)tileGroup.size()), nn(n * n), emptyTile(nn - 1),
	tileGroupMap(nn, emptyTile), db(nPr(nn, r), DB_UNDEFINED), pId(r), cId(nn, r) {
	assert(n == 4);
	for (int i = 0; i < r; i++) {
		tileGroupMap[tileGroup[i]] = i;
	}
	for (int i = 0; i < nn; i++) {
		if (tileGroupMap[i] == emptyTile) {
			dummyTile = i;
			break;
		}
	}
}

int DisjointPatternDB::getStateIndex(PuzzleState state) const {
	vector<int> combi, permu;
	for (int i = 0; i < nn; i++) {
		int x = state.get(i);
		if (tileGroupMap[x] != emptyTile) {
			combi.push_back(i);
			permu.push_back(tileGroupMap[x]);
		}
	}

	assert(combi.size() == r);
	sort(combi.begin(), combi.end());

	BitBlock<int, 3> bbPermu;
	for (int i = 0; i < r; i++) {
		bbPermu.set(i, permu[i]);
	}
	int permuIndex = pId.getIndex(bbPermu);

	BitBlock<int, 4> bbCombi;
	for (int i = 0; i < r; i++) {
		bbCombi.set(i, combi[i]);
	}
	int combiIndex = cId.getIndex(bbCombi);

	return combiIndex * factorialMap[r] + permuIndex;
}

void DisjointPatternDB::storeToFile(const char *path) const {
	ofstream o(path, ios::out | ios::binary);
	o.write((const char *)db.data(), db.size());
	o.close();
}

void DisjointPatternDB::readFromFile(const char *path) {
	ifstream f(path, ios::in | ios::binary);
	f.read((char *)db.data(), db.size());
	f.close();
}

int DisjointPatternDB::getDBValue(PuzzleState state) const {
	return db[getStateIndex(state)];
}
