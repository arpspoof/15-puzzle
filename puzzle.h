#pragma once

// For 24-puzzle please change to __int128
typedef unsigned long long PuzzleStateStorage;

// 4 for 8-puzzle and 15-puzzle, 5 for 24-puzzle
constexpr int ELEMENT_BITS = 4;
constexpr int ELEMENT_MASK = 15;

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
	int zeroPos;
	PuzzleState state;
public:
	int n;
	Puzzle(int n);
	Puzzle(int n, int *permutation);
public:
	// test whether can move blank(0) u/d/l/r
	bool canMove(char op);
	void move(char op);
private:
	bool isValid();
};
