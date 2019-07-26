#include "solver.h"

#include <cassert>
#include <unordered_set>
#include <memory.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

static const int GOAL_NODE = -1;
static const int INF = 0x7FFFFFFF;

static char path[256] = { 0 };
static char operationList[] = { 'u','d','l','r' };
static char reverse_operationList[] = { 'd','u','r','l' };

static unsigned long long nNodesExpanded = 0;
static int maxSearchLength = -1;
static int pathPointer = 0;
static unordered_set<PuzzleStateStorage> exploredSet;

int IDA(Puzzle &p, Heuristic h, int lengthToCurrentState) {
	int estimatedCostCurrent = lengthToCurrentState + h(p);
	if (estimatedCostCurrent > maxSearchLength) {
		return estimatedCostCurrent;
	}
	if (p.isInGoalState()) {
		return GOAL_NODE;
	}
	int minEstimatedCost = INF;
	for (int i = 0; i < 4; i++) {
		char op = operationList[i];
		if (!p.canMove(op)) {
			continue;
		}
		p.move(op);
		PuzzleStateStorage newStateHash = p.state.getStorage();
		if (exploredSet.find(newStateHash) == exploredSet.end()) {
			nNodesExpanded++;
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
		char reverseOp = reverse_operationList[i];
		p.move(reverseOp);
	}
	return minEstimatedCost;
}

SearchResult IDA(Puzzle p, Heuristic h) {
	pathPointer = 0;
	memset(path, 0, sizeof(path));

	maxSearchLength = h(p);

	exploredSet.clear();
	exploredSet.insert(p.state.getStorage());

	int nInterations = 0;
	unsigned long long nTotalNodesExpanded = 0;

	printf("initial h value is %d\n", h(p));
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	while (1) {
		nInterations++;
		nNodesExpanded = 0;

		printf("iteration %d, maxSearch = %d\n", nInterations, maxSearchLength);

		int code = IDA(p, h, 0);
		assert(code != INF);

		nTotalNodesExpanded += nNodesExpanded;

		if (code == GOAL_NODE) break;
		maxSearchLength = code;
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();

	SearchResult res;
	res.length = pathPointer;
	res.path = path;
	res.nIterations = nInterations;
	res.nNodesExpanded = nTotalNodesExpanded;
	res.nNodesInLastInteration = nNodesExpanded;
	res.runTime = duration_cast<microseconds>(t2 - t1).count() / (double)1e3;

	return res;
}
