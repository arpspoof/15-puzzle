#include "index.h"
#include <cassert>

static int factorialMap[] = {
	1, 1, 2, 6, 24, 120, 720, 5040, 40320
};

PermutationIndex::PermutationIndex(int n) :n(n) {
	assert(n >= 1 && n <= 8);
	arrayMap = new BitBlock<int, 3, 7>[factorialMap[n]];
}

const BitBlock<int, 3, 7>& PermutationIndex::getPermutation(int index) const {
	return arrayMap[index];
}
