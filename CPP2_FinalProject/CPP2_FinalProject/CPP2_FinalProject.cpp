/*
	Project By: Isaac N.

	File: Main Function File
*/

#include "MainHeader.h"

int main() {
	srand(time(NULL)); // Getting a random number seed [REQUIRED ON RUNTIME FOR rand() TO WORK]

	bool isFirst = true;
	bool isRetrying = true;
	while (isRetrying != false) {
		int choice = StartingMessage(isRetrying, isFirst);
		isFirst = false;

		MazeBase currMaze = FindMaze(choice);

		MovementControls(&currMaze, &isRetrying);
	}

	return 0;
}