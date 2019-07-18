﻿// a5.cpp : Defines the entry point for the application.
//

#include "a5.h"
#include "puzzle.h"
#include "solver.h"

#include <cassert>
#include <string>

using namespace std;

int manhattan(PuzzleState state) {
	int value = 0;
	for (int i = 0; i < 9; i++) {
		int element = state.get(i);
		if (element == 8) {
			continue;
		}
		int r1 = i / 3, c1 = i % 3;
		int r2 = element / 3, c2 = element % 3;
		value += abs(r1 - r2) + abs(c1 - c2);
	}
	return value;
}

char getC(int v) {
	if (v == 8) return ' ';
	else return v + '0';
}

void display(Puzzle& p) {
	PuzzleState s = p.state;
	printf("%c %c %c\n%c %c %c\n%c %c %c\n\n",
		getC(s.get(0)), getC(s.get(1)), getC(s.get(2)),
		getC(s.get(3)), getC(s.get(4)), getC(s.get(5)), 
		getC(s.get(6)), getC(s.get(7)), getC(s.get(8))
	);
}

int main()
{
	PuzzleState s;
	for (int i = 0; i < 10000; i++) {
		Puzzle p = getRandomPuzzle(3);
		assert(p.isSolvable());
		SearchResult res = IDA(p, manhattan);
		printf("solved problem %d, solution = %d\n", i, res.length);
		char *path = res.path;
		assert(p.verifySolution(path));
	}

	system("pause");
	return 0;
}