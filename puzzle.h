#pragma once

// For 24-puzzle please change to __int128
typedef unsigned long long PuzzleStateStorage;

// 4 for 8-puzzle and 15-puzzle, 5 for 24-puzzle
constexpr int ELEMENT_BITS = 4;
constexpr int ELEMENT_MASK = 15;

constexpr PuzzleStateStorage GOAL_4 = 0xFEDCBA9876543210;
constexpr PuzzleStateStorage GOAL_3 = 0x0000000876543210;

class PuzzleState {
private:
	PuzzleStateStorage state;
public:
	PuzzleState();
public:
	int get(int i);
	void set(int i, int v);
	void exchange(int i, int j);
	PuzzleStateStorage getStorage();
};

class Puzzle {
	int nn;
	int emptyPos;
public:
	int n;
	PuzzleState state;
	Puzzle(int n, int *permutation);
public:
	bool canMove(char op);
	void move(char op);
	bool isInGoalState();
	bool isSolvable();
	bool verifySolution(char *ops);
private:
	bool isValid();
};

Puzzle getRandomPuzzle(int n);
