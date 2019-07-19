#pragma once

#include "bit_blocks.h"

#include <vector>
#include <unordered_map>
#include <cassert>
#include <algorithm>

constexpr int MAX_PERMUTATION_N = 8;

constexpr int factorialMap[] = {
	1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
};

class PermutationIndex {
	int n;
	std::vector<BitBlock<int, 3>> permutationMap;
	std::unordered_map<int, int> indexMap;
public:
	PermutationIndex(int n) :n(n) {
		assert(n >= 1 && n <= MAX_PERMUTATION_N);
		permutationMap = std::vector<BitBlock<int, 3>>(factorialMap[n]);

		int a[MAX_PERMUTATION_N];
		for (int i = 0; i < n; i++) {
			a[i] = i;
		}

		for (int i = 0; i < factorialMap[n]; i++) {
			for (int j = 0; j < n; j++) {
				permutationMap[i].set(j, a[j]);
			}
			indexMap[permutationMap[i].getStorage()] = i;
			std::next_permutation(a, a + n);
		}
	}
	const BitBlock<int, 3>& getPermutation(int index) const {
		return permutationMap[index];
	}
	int getIndex(const BitBlock<int, 3>& permutation) const {
		return indexMap.find(permutation.getStorage())->second;
	}
};

template <int blockSize> class CombinationIndex {
	int n, r;
	std::vector<BitBlock<int, blockSize>> combinationMap;
	std::unordered_map<int, int> indexMap;
public:
	CombinationIndex() {}
	CombinationIndex(int n, int r) :n(n), r(r) {
		assert((int)ceil(log2(n)) * r <= 32);

		int nPr = 1;
		for (int i = n; i > n - r; i--) {
			nPr *= i;
		}
		combinationMap = std::vector<BitBlock<int, blockSize>>(nPr / factorialMap[r]);

		int index = 0;
		std::vector<int> chosenList;

		build(chosenList, index, r, 0);
	}
	const BitBlock<int, blockSize>& getCombination(int index) const {
		return combinationMap[index];
	}
	int getIndex(const BitBlock<int, blockSize>& combination) const {
		return indexMap.find(combination.getStorage())->second;
	}
private:
	void build(std::vector<int>& chosenList, int& index, int choose, int from) {
		if (choose == 0) {
			if (chosenList.size() != r) {
				int a = 0;
			}
			assert(chosenList.size() == r);
			for (int i = 0; i < r; i++) {
				combinationMap[index].set(i, chosenList[i]);
			}
			indexMap[combinationMap[index].getStorage()] = index;
			index++;
			return;
		}

		if (n - 1 - from >= choose - 1) {
			chosenList.push_back(from);
			build(chosenList, index, choose - 1, from + 1);
			chosenList.pop_back();
		}
		
		if (n - 1 - from >= choose) {
			build(chosenList, index, choose, from + 1);
		}
	}
};
