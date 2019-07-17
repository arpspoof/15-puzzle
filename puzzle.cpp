#include "puzzle.h"

#include <cassert>
#include <vector>

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

Puzzle::Puzzle(int n) :n(n), nn(n * n), zeroPos(0) {
	for (int i = 0; i < nn; i++) {
		state.set(i, i);
	}
}

Puzzle::Puzzle(int n, int *permutation) :n(n), nn(n * n), zeroPos(-1) {
	for (int i = 0; i < nn; i++) {
		state.set(i, permutation[i]);
		if (permutation[i] == 0) {
			zeroPos = i;
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
	return x >> low && x < high;
}

bool Puzzle::canMove(char op) {
	switch (op) {
	case 'u':
		return within(zeroPos, n, nn);
	case 'd':
		return within(zeroPos, 0, nn - n);
	case 'l':
		return within(zeroPos % n, 1, n);
	case 'r':
		return within(zeroPos % n, 0, n - 1);
	default:
		assert(false);
	}
	return false;
}

void Puzzle::move(char op) {
	assert(canMove(op));
	switch (op) {
	case 'u':
		return state.exchange(zeroPos, zeroPos - n);
	case 'd':
		return state.exchange(zeroPos, zeroPos + n);
	case 'l':
		return state.exchange(zeroPos, zeroPos - 1);
	case 'r':
		return state.exchange(zeroPos, zeroPos + 1);
	}
}
