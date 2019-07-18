#pragma once

#include "puzzle.h"

typedef int (*Heuristic)(PuzzleState state);

struct SearchResult
{
	int length;
	char *path;
	double runTime;
	unsigned long long nIterations;
	unsigned long long nNodesExpanded;
	SearchResult() :path(nullptr), nIterations(0), nNodesExpanded(0),
		runTime(0), length(0) {}
};

SearchResult IDA(Puzzle p, Heuristic h);
