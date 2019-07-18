#pragma once

#include "puzzle.h"

typedef int (*Heuristic)(Puzzle state);

struct SearchResult
{
	int length;
	char *path;
	double runTime;
	int nIterations;
	unsigned long long nNodesExpanded;
	unsigned long long nNodesInLastInteration;
	SearchResult() :path(nullptr), nIterations(0), nNodesExpanded(0),
		runTime(0), length(0), nNodesInLastInteration(0) {}
};

SearchResult IDA(Puzzle p, Heuristic h);
