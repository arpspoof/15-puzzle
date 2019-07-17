// a5.cpp : Defines the entry point for the application.
//

#include "a5.h"
#include "puzzle.h"

using namespace std;

int main()
{
	PuzzleState state;
	for (int i = 0; i < 16; i++) {
		state.set(15 - i, i);
	}
	for (int i = 0; i < 16; i++) {
		cout << state.get(i) << endl;
	}
	system("pause");
	return 0;
}
