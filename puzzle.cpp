#include "puzzle.h"

#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

Puzzle::Puzzle(int n, const int *permutation) :n(n), nn(n * n), emptyPos(-1) {
	for (int i = 0; i < nn; i++) {
		state.set(i, permutation[i]);
		if (permutation[i] == nn - 1) {
			emptyPos = i;
		}
	}
	assert(isValid());
}

bool Puzzle::isValid() const {
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

bool Puzzle::canMove(char op) const {
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

bool Puzzle::isInGoalState() const {
	switch (n) {
	case 4:
		return state.getStorage() == GOAL_4;
	case 3:
		return state.getStorage() == GOAL_3;
	default:
		assert(false);
	}
	return false;
}

bool Puzzle::isSolvable() const {
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
	int parity = nInversions & 1;
	if (n & 1) return parity == 0;
	else return ((emptyPos / n) & 1) != parity;
}

bool Puzzle::verifySolution(const char *ops) const {
	Puzzle copy(*this);
	while (*ops != 0) {
		copy.move(*ops);
		ops++;
	}
	return copy.isInGoalState();
}

string Puzzle::toString() const {
	string s = "";
	for (int i = 0; i < nn; i++) {
		s += 'a' + state.get(i);
	}
	return s;
}

Puzzle::Puzzle(const string &str) {
	n = (int)str.length();
	nn = n * n;
	for (int i = 0; i < nn; i++) {
		int x = str[i] - 'a';
		state.set(i, x);
		if (x == nn - 1) {
			emptyPos = x;
		}
	}
	assert(isValid());
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
