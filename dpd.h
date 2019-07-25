#pragma once

#include "puzzle.h"
#include "index.h"

#include <vector>

class DisjointPatternDB {
	int n, r, nn, emptyTile, dummyTile;
	std::vector<unsigned char> tileGroupMap;
	std::vector<unsigned char> db;
	PermutationIndex<3> pId;
	CombinationIndex<4> cId;
	Bitmap searchHash;
	std::vector<std::vector<std::pair<PuzzleState, unsigned char>>> dataArray;
	std::string name;
public:
	DisjointPatternDB(int n, const std::vector<int> &tileGroup);
public:
	void compute(int nThreads, int batchSize);
	void storeToFile(const char *path) const;
	void readFromFile(const char *path);
	int getDBValue(PuzzleState state);
private:
	int getStateIndex(PuzzleState state) const;
	unsigned int getSearchStateIndex(PuzzleState state) const;
	bool checkMove(int pos, char dir) const;
	PuzzleState moveAndCopy(PuzzleState s, int pos, char dir) const;
	void bfs_thread(int fileId, int dataId);
	std::string getFilePathById(int Id) const;
};
