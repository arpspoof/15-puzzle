#include "solver.h"

#include <cassert>
#include <unordered_set>
#include <memory>

using namespace std;

static const int GOAL_NODE = -1;
static const int INF = 0x7FFFFFFF;

static char path[256] = { 0 };
static char operationList[] = { 'u','d','l','r' };

static int maxSearchLength = -1;
static int pathPointer = 0;
static unordered_set<PuzzleStateStorage> exploredSet;

static char getOppositeOperation(char op) {
	switch (op) {
	case 'u': return 'd';
	case 'd': return 'u';
	case 'l': return 'r';
	case 'r': return 'l';
	}
	assert(false);
	return '\0';
}

int IDA(Puzzle &p, Heuristic h, int lengthToCurrentState) {
	int estimatedCostCurrent = lengthToCurrentState + h(p.state);
	if (estimatedCostCurrent > maxSearchLength) {
		return estimatedCostCurrent;
	}
	if (p.isInGoalState()) {
		return GOAL_NODE;
	}
	int minEstimatedCost = INF;
	for (int i = 0; i < 4; i++) {
		char op = operationList[i];
		char reverseOp = getOppositeOperation(op);
		if (!p.canMove(op)) {
			continue;
		}
		p.move(op);
		assert(p.isSolvable());
		PuzzleStateStorage newStateHash = p.state.getStorage();
		if (exploredSet.find(newStateHash) == exploredSet.end()) {
			exploredSet.insert(newStateHash);
			path[pathPointer++] = op;
			int estimatedCostChild = IDA(p, h, lengthToCurrentState + 1);
			if (estimatedCostChild == GOAL_NODE) {
				return GOAL_NODE;
			}
			if (estimatedCostChild < minEstimatedCost) {
				minEstimatedCost = estimatedCostChild;
			}
			path[--pathPointer] = 0;
			exploredSet.erase(newStateHash);
		}
		p.move(reverseOp);
		assert(p.isSolvable());
	}
	return minEstimatedCost;
}

SearchResult IDA(Puzzle p, Heuristic h) {
	pathPointer = 0;
	memset(path, 0, sizeof(path));

	maxSearchLength = h(p.state);

	exploredSet.clear();
	exploredSet.insert(p.state.getStorage());

	while (1) {
		int code = IDA(p, h, 0);
		assert(code != INF);

		if (code == GOAL_NODE) break;
		maxSearchLength = code;
	}

	SearchResult res;
	res.length = pathPointer;
	res.path = path;
	return res;
}
