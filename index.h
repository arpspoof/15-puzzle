#pragma once

#include "bit_blocks.h"

class PermutationIndex {
	int n;
	BitBlock<int, 3, 7> *arrayMap;
public:
	PermutationIndex(int n);
	const BitBlock<int, 3, 7>& getPermutation(int index) const;
};