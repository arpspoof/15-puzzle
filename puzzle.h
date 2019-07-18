#pragma once

#include "bit_blocks.h"

// For 24-puzzle please change to __int128
typedef unsigned long long PuzzleStateStorage;
typedef BitBlock<PuzzleStateStorage, 4, 15> PuzzleState;

constexpr PuzzleStateStorage GOAL_4 = 0xFEDCBA9876543210;
constexpr PuzzleStateStorage GOAL_3 = 0x0000000876543210;

class Puzzle {
	int nn;
	int emptyPos;
public:
	int n;
	PuzzleState state;
	Puzzle(int n, const int *permutation);
public:
	bool canMove(char op) const;
	void move(char op);
	bool isInGoalState() const;
	bool isSolvable() const;
	bool verifySolution(const char *ops) const;
private:
	bool isValid() const;
};

Puzzle getRandomPuzzle(int n);
