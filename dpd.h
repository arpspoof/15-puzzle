#pragma once

#include "puzzle.h"
#include "index.h"

#include <vector>

class DisjointPatternDB {
	int n, r, nn, emptyTile, dummyTile;
	PuzzleStateStorage goalMask, goalValue;
	std::vector<unsigned char> tileGroupMap;
	std::vector<unsigned char> db;
	PermutationIndex pId;
	CombinationIndex<4> cId4;
	CombinationIndex<5> cId5;
public:
	DisjointPatternDB(int n, const std::vector<int> &tileGroup);
public:
	void compute();
	void storeToFile(const char *path) const;
	void readFromFile(const char *path);
	int getDBValue(PuzzleState state);
//private:
	int getStateIndex(PuzzleState state) const;
	bool checkMove(int pos, char dir) const;
	PuzzleState moveAndCopy(PuzzleState s, int pos, char dir) const;
	int manhattan(const PuzzleState &s) const;
	int aStar(const PuzzleState &s) const;
};
