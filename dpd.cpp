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
	tileGroupMap(nn, emptyTile), db(nPr(nn, r + 1), DB_UNDEFINED), pId(r + 1) {
	assert(n == 4 || n == 5);
	for (int i = 0; i < r; i++) {
		tileGroupMap[tileGroup[i]] = i + 1;
	}
	tileGroupMap[emptyTile] = 0;
	for (int i = 0; i < nn; i++) {
		if (tileGroupMap[i] == emptyTile) {
			dummyTile = i;
			break;
		}
	}
	if (n == 4) cId4 = CombinationIndex<4>(nn, r + 1);
	if (n == 5) cId5 = CombinationIndex<5>(nn, r + 1);
}

bool DisjointPatternDB::checkMove(PuzzleState s, int pos, char dir) const {
	switch (dir) {
	case 'u':
		return (pos / n != 0);
	case 'd':
		return (pos / n != n - 1);
	case 'l':
		return (pos % n != 0);
	case 'r':
		return (pos % n != n - 1);
	default:
		assert(false);
		return false;
	}
}

PuzzleState DisjointPatternDB::moveAndCopy(PuzzleState s, int pos, char dir) const {
	assert(checkMove(s, pos, dir));
	PuzzleState res(s);
	switch (dir) {
	case 'u':
		res.exchange(pos, pos - n);
		break;
	case 'd':
		res.exchange(pos, pos + n);
		break;
	case 'l':
		res.exchange(pos, pos - 1);
		break;
	case 'r':
		res.exchange(pos, pos + 1);
		break;
	default:
		assert(false);
	}
	return res;
}

void DisjointPatternDB::compute() {
	PuzzleState root;
	for (int i = 0; i < nn; i++) {
		if (tileGroupMap[i] != emptyTile) root.set(i, i);
		else root.set(i, dummyTile);
	}

	typedef pair<unsigned char, PuzzleStateStorage> pp;
	priority_queue<pp, vector<pp>, greater<pp>> frontiers;
	frontiers.push(make_pair(0, root.getStorage()));
	db[getStateIndex(root)] = 0;

	char actionList[] = { 'u', 'd', 'l', 'r' };

	int filledCount = 1;
	while (!frontiers.empty()) {
		auto p = frontiers.top();
		PuzzleState s(p.second);
		frontiers.pop();
		for (int i = 0; i < nn; i++) {
			if (s.get(i) != emptyTile) continue;
			for (char c : actionList) {
				if (checkMove(s, i, c)) {
					PuzzleState copy = moveAndCopy(s, i, c);
					int newIndex = getStateIndex(copy);
					if (db[newIndex] == DB_UNDEFINED) {
						int newCost = p.first + (copy.get(i) != dummyTile);
						db[newIndex] = newCost;
						frontiers.push(make_pair(newCost, copy.getStorage()));
						filledCount++;
					/*	if (db.size() > 10000000 && (filledCount - 2) % 10000000 == 0) {
							printf("computing db progress: %f ...\n",
								(double)filledCount / db.size());
						}*/
					}
				}
			}
			break;
		}
	}

	assert(filledCount == (int)db.size());
	printf("computed %d entries\n", filledCount);
}

int DisjointPatternDB::getStateIndex(PuzzleState state) const {
	vector<int> combi, permu;
	for (int i = 0; i < nn; i++) {
		int x = state.get(i);
		if (tileGroupMap[x] != emptyTile || x == emptyTile) {
			combi.push_back(i);
			permu.push_back(tileGroupMap[x]);
		}
	}

	assert(combi.size() == r + 1);

	// group size 7 or less !
	BitBlock<int, 3> bbPermu;
	for (int i = 0; i <= r; i++) {
		bbPermu.set(i, permu[i]);
	}
	int permuIndex = pId.getIndex(bbPermu);
	int combiIndex;

	if (n == 4) {
		BitBlock<int, 4> bbCombi;
		for (int i = 0; i <= r; i++) {
			bbCombi.set(i, combi[i]);
		}
		combiIndex = cId4.getIndex(bbCombi);
	}
	else if (n == 5) {
		BitBlock<int, 5> bbCombi;
		for (int i = 0; i <= r; i++) {
			bbCombi.set(i, combi[i]);
		}
		combiIndex = cId5.getIndex(bbCombi);
	}

	return combiIndex * factorialMap[r + 1] + permuIndex;
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

int DisjointPatternDB::getDBValue(PuzzleState state) {
	return db[getStateIndex(state)];
}
