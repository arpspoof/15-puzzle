#pragma once

#include <vector>
#include <string>

void computeDpd(
	int n,
	const std::string &storePath,
	const std::vector<int> &patternTileGroup,
	int nThreads
);
