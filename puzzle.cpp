#include "puzzle.h"

#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

PuzzleState::PuzzleState() :state(0) {
}

int PuzzleState::get(int i) {
	return (state >> (i * ELEMENT_BITS)) & (PuzzleStateStorage)ELEMENT_MASK;
}

void PuzzleState::set(int i, int v) {
	state ^= (PuzzleStateStorage)((v & ELEMENT_MASK) ^ get(i)) << (i * ELEMENT_BITS);
}

void PuzzleState::exchange(int i, int j) {
	int xi = get(i), xj = get(j);
	set(i, xj);
	set(j, xi);
}

PuzzleStateStorage PuzzleState::getStorage() {
	return state;
}

Puzzle::Puzzle(int n, int *permutation) :n(n), nn(n * n), emptyPos(-1) {
	for (int i = 0; i < nn; i++) {
		state.set(i, permutation[i]);
		if (permutation[i] == nn - 1) {
			emptyPos = i;
		}
	}
	assert(isValid());
}

bool Puzzle::isValid() {
	vector<int> mp(nn, 0);
	for (int i = 0; i < nn; i++) {
		int element = state.get(i);
		if (mp[element] == 1) {
			return false;
		}
		mp[element] = 1;
	}
	return true;
}

static bool within(int x, int low, int high) {
	return x >= low && x < high;
}

bool Puzzle::canMove(char op) {
	switch (op) {
	case 'u':
		return within(emptyPos, n, nn);
	case 'd':
		return within(emptyPos, 0, nn - n);
	case 'l':
		return within(emptyPos % n, 1, n);
	case 'r':
		return within(emptyPos % n, 0, n - 1);
	default:
		assert(false);
	}
	return false;
}

void Puzzle::move(char op) {
	assert(canMove(op));
	switch (op) {
	case 'u':
		state.exchange(emptyPos, emptyPos - n);
		emptyPos = emptyPos - n;
		break;
	case 'd':
		state.exchange(emptyPos, emptyPos + n);
		emptyPos = emptyPos + n;
		break;
	case 'l':
		state.exchange(emptyPos, emptyPos - 1);
		emptyPos = emptyPos - 1;
		break;
	case 'r':
		state.exchange(emptyPos, emptyPos + 1);
		emptyPos = emptyPos + 1;
		break;
	}
}

bool Puzzle::isInGoalState() {
	switch (n) {
	case 4:
		return state.getStorage() == GOAL_4;
	case 3:
		return state.getStorage() == GOAL_3;
	default:
		assert(false);
	}
}

bool Puzzle::isSolvable() {
	vector<int> nums(nn);
	for (int i = 0; i < nn; i++) {
		nums[i] = state.get(i);
	}
	int nInversions = 0;
	for (int i = 0; i < nn; i++) {
		if (nums[i] == nn - 1) continue;
		for (int j = i + 1; j < nn; j++) {
			if (nums[j] == nn - 1) continue;
			if (nums[j] < nums[i]) {
				nInversions++;
			}
		}
	}
	return (nInversions & 1) == 0;
}

bool Puzzle::verifySolution(char *ops) {
	Puzzle copy(*this);
	while (*ops != 0) {
		copy.move(*ops);
		ops++;
	}
	return copy.isInGoalState();
}

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

Puzzle getRandomPuzzle(int n) {
	int nn = n * n;
	vector<pair<double, int>> ar;
	while (true) {
		ar.clear();
		for (int i = 0; i < nn; i++) {
			ar.push_back(make_pair(dist(mt), i));
		}
		sort(ar.begin(), ar.end());
		vector<int> nums;
		for (int i = 0; i < nn; i++) {
			nums.push_back(ar[i].second);
		}
		Puzzle p(n, nums.data());
		if (p.isSolvable()) {
			return p;
		}
	}
}
