#pragma once
/*
	Project By: Isaac N.

	File: MAIN HEADER FILE
*/


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>

#include "conio.h"

using namespace std;
using namespace std::this_thread;
using namespace chrono_literals;

#define ANSI_RESET "\033[0m";
#define ANSI_BLACK "\033[30m";
#define ANSI_RED "\033[31m";
#define ANSI_GREEN "\033[32m";
#define ANSI_YELLOW "\033[33m";
#define ANSI_BLUE "\033[34m";
#define ANSI_PURPLE "\033[35m";
#define ANSI_CYAN "\033[36m";
#define ANSI_WHITE "\033[37m";


//*********	//\\ MAPS //\\ *********\\

map<string, int> intMazeStats{
	{"PercentForIntersection", 10},
};

map<string, string> stringMazeStats{
	{"Wall", " X "},
	{"Space", "   "},
	{"Player", ">P<"},
	{"StartPos", " 1 "},
	{"EndPos", " 2 "},
};

//*********	//\\ MAPS END //\\ ********* \\



//*********	//\\ CLASSES //\\ ********* \\

class MazeBase {
public:
	string Name;
	double Rating;
	int rowSize;
	int colSize;
	vector<vector<string>> v_Maze; // [Y][X] Y = Up/Down || X = Left/Right

	// Variables when maze is being generated
	int rowStart;
	bool reachedBottom = false;
	bool generatingMaze;

	int startY;
	int startX;

	int currY;
	int currX;

	// Variables when maze is being played
	bool playing;
	int plrSightRange = 2;

	int plrPosY;
	int plrPosX;

	MazeBase();

	~MazeBase() {};

	// Maze One constructor with params
	MazeBase(string name, double rating, int rowSize, int colSize) {
		this->Name = name;
		this->Rating = rating;
		this->rowSize = rowSize;
		this->colSize = colSize;

		v_Maze.resize(this->colSize);

		for (int i = 0; i < this->colSize; i++) {
			v_Maze[i].resize(this->rowSize);
			for (int j = 0; j < this->rowSize; j++) {
				v_Maze[i][j] = stringMazeStats.at("Wall");
			}
		}
	};

	// Prints current maze vector from the current class
	void PrintMaze() {
		for (int C = 0; C < colSize; C++) {
			cout << "\n\n";
			for (int R = 0; R < rowSize; R++) {
				if (v_Maze[C][R] == stringMazeStats.at("Player")) {
					cout << ANSI_GREEN;
					cout << v_Maze[C][R];
					cout << ANSI_RESET;
				}
				else {
					cout << ANSI_CYAN;
					cout << v_Maze[C][R];
					cout << ANSI_RESET;
				}
				
			}
		}
		cout << ANSI_RESET;
	}

	void GenerateUp(int far) {
		int holdY = currY; int holdX = currX;
		for (int i = 0; i < far; i++) {
			currY -= 1; this->currY = holdY;
			if (this->currY < this->colSize && this->currY >= 0) {
				v_Maze[currY][currX] = stringMazeStats.at("Space");
			}
			else {
				this->currY += 1;
				break;
			}
		}
	}
	void GenerateDown(int far) {
		int holdY = currY; int holdX = currX;
		for (int i = 0; i < far; i++) {
			holdY += 1; this->currY = holdY;
			if (this->currY < this->colSize - 1 && this->currY >= 0) {
				v_Maze[currY][currX] = stringMazeStats.at("Space");
			}
			else if (this->currY == this->colSize - 1) {
				if (this->reachedBottom != true) {
					v_Maze[currY][currX] = stringMazeStats.at("Space");
					this->reachedBottom = true;
				}
			}
			else {
				this->currY -= 1;
				break;
			}
		}
	}
	void GenerateRight(int far) {
		int holdY = currY; int holdX = currX;
		for (int i = 0; i < far; i++) {
			holdX += 1; this->currX = holdX;
			if (this->currX < this->rowSize && this->currX >= 0) {
				v_Maze[currY][currX] = stringMazeStats.at("Space");
			}
			else {
				this->currX -= 1;
				break;
			}
		}
	}
	void GenerateLeft(int far) {
		int holdY = currY; int holdX = currX;
		for (int i = 0; i < far; i++) {
			holdX -= 1; this->currX = holdX;
			if (this->currX < this->rowSize && this->currX >= 0) {
				v_Maze[currY][currX] = stringMazeStats.at("Space");
			}
			else {
				this->currX += 1;
				break;
			}
		}
	}
	
	// Generates first row within the vector
	void GenerateFirstRow() {
		this->generatingMaze = true;
		this->rowStart = 0 + (rand() % (this->rowSize));
		v_Maze[0][this->rowStart] = stringMazeStats.at("Space");
		this->currY = 0;
		this->currX = this->rowStart;

		this->startY = this->currY;
		this->startX = this->currX;
	}

	// Generates solvable path 
	void GeneratePath() {
		int TimeDown = 1 + (rand() % (4));
		GenerateDown(TimeDown);

		string lastDone = "Down"; // Options are "Up" "Down" "Left" "Right"
		while (this->reachedBottom == false) {
			int randNumOf4 = 1 + (rand() % (4));
			int randNumOf6 = 1 + (rand() % static_cast<int>(ceil(-2.00 * log10(static_cast<int>(static_cast<double>(this->Rating * 8.00))))) + (this->Rating * 2));
			if (randNumOf4 == 1 && lastDone != "Up") { // For Up
				lastDone = "Up";
				GenerateUp(randNumOf6);
			} else if (randNumOf4 == 2 && lastDone != "Down"){ // For Down
				lastDone = "Down";
				GenerateDown(randNumOf6);
			} else if (randNumOf4 == 3 && lastDone != "Left") { // For Left
				lastDone = "Left";
				GenerateLeft(randNumOf6);
			} else if (randNumOf4 == 4 && lastDone != "Right") { // For Right
				lastDone = "Right";
				GenerateRight(randNumOf6);
			}
		}
	}

	// Creates random branches after the first solvable path is created
	void CreateBranchPath() {
		int randNumOf7 = 1 + (rand() % (7));
		string lastDone = ""; // Options are "Up" "Down" "Left" "Right"

		for (int i = 0; i < randNumOf7; i++) {
			if ((this->currY != 0 && this->currY != 1) && (this->currY < this->colSize - 2) && (this->currX != 0 && this->currX != 1) && (this->currX < this->rowSize - 2)) {
				int randNumOf4 = 1 + (rand() % (4));
				int randNumOf6 = 1 + (rand() % static_cast<int>(ceil(-2.00 * log10(static_cast<int>(static_cast<double>(this->Rating * 8.00))))) + (this->Rating * 2));
				if (randNumOf4 == 1 && lastDone != "Up" && v_Maze[currY - 1][currX] != stringMazeStats.at("Space") && v_Maze[currY - 1][currX - 1] != stringMazeStats.at("Space") && v_Maze[currY - 1][currX + 1] != stringMazeStats.at("Space")) { // For Up
					lastDone = "Up";
					GenerateUp(randNumOf6);
				}
				else if (randNumOf4 == 2 && lastDone != "Down" && v_Maze[currY + 1][currX] != stringMazeStats.at("Space") && v_Maze[currY + 1][currX - 1] != stringMazeStats.at("Space") && v_Maze[currY + 1][currX + 1] != stringMazeStats.at("Space")) { // For Down
					lastDone = "Down";
					GenerateDown(randNumOf6);
				}
				else if (randNumOf4 == 3 && lastDone != "Left" && v_Maze[currY][currX - 1] != stringMazeStats.at("Space") && v_Maze[currY - 1][currX - 1] != stringMazeStats.at("Space") && v_Maze[currY + 1][currX - 1] != stringMazeStats.at("Space")) { // For Left
					lastDone = "Left";
					GenerateLeft(randNumOf6);
				}
				else if (randNumOf4 == 4 && lastDone != "Right" && v_Maze[currY][currX + 1] != stringMazeStats.at("Space") && v_Maze[currY - 1][currX + 1] != stringMazeStats.at("Space") && v_Maze[currY + 1][currX + 1] != stringMazeStats.at("Space")) { // For Right
					lastDone = "Right";
					GenerateRight(randNumOf6);
				}
				else {
					
					return;
				}
			}

		}	
	}

	// Runs through a for loop that iterates through the entire maze calls CreateBranchPath upon a successful attempt to create a branch
	void GenerateRandPaths() {
		for (int C = 0; C < colSize; C++) {
			for (int R = 0; R < rowSize; R++) {
				this->currY = C; this->currX = R;
				int randNumOf100 = 1 + (rand() % (100));
				if (v_Maze[this->currY][this->currX] == stringMazeStats.at("Space") && this->currY < this->colSize - 2 && this->currY > 1 && this->currX < this->rowSize - 2 && this->currX > 1 && randNumOf100 <= intMazeStats.at("PercentForIntersection")) {
					this->CreateBranchPath();
				}
			}
		}
	}

	// Kicks starts the creating of the maze within the class MazeBase
	void GenerateMazeAndPlay() {
		this->GenerateFirstRow();
		this->GeneratePath();
		this->GenerateRandPaths();
		this->GenerateRandPaths();
		this->GenerateRandPaths();
		this->StartPlaying();
	}

	void PrintMazeAroundPlr() {
		int startX = max(0, plrPosX - plrSightRange);
		int endX = min(rowSize - 1, plrPosX + plrSightRange);
		int startY = max(0, plrPosY - plrSightRange);
		int endY = min(colSize - 1, plrPosY + plrSightRange);

		for (int C = startY; C <= endY; C++) {
			cout << "\n\n";
			for (int R = startX; R <= endX; R++) {
				if (C == plrPosY && R == plrPosX) {
					cout << ANSI_GREEN;
				}
				else {
					cout << ANSI_CYAN;
				}
				cout << v_Maze[C][R] << ANSI_RESET;
			}
		}
	}

	void GoUp() {
		if (this->plrPosY != 0) {
			if (this->v_Maze[this->plrPosY - 1][this->plrPosX] != stringMazeStats.at("Wall")) {
				this->v_Maze[this->plrPosY][this->plrPosX] = stringMazeStats.at("Space");
				this->v_Maze[this->plrPosY - 1][this->plrPosX] = stringMazeStats.at("Player");
				this->plrPosY -= 1;
				PrintMazeAroundPlr();
			} else {
				PrintMazeAroundPlr();
				cout << ANSI_RED;
				cout << "\n\nMovement Failed[YOU HIT A WALL BOZO]" << endl;
				cout << ANSI_RESET;
			}
		} else {
			PrintMazeAroundPlr();
			cout << ANSI_RED;
			cout << "\n\nMovement Failed[YOU CANT GET OUT OF MAZE]" << endl;
			cout << ANSI_RESET;
		}
	}
	void GoDown() {
		if (this->plrPosY != this->colSize - 1) {
			if (this->v_Maze[this->plrPosY + 1][this->plrPosX] != stringMazeStats.at("Wall")) {
				this->v_Maze[this->plrPosY][this->plrPosX] = stringMazeStats.at("Space");
				this->v_Maze[this->plrPosY + 1][this->plrPosX] = stringMazeStats.at("Player");
				this->plrPosY += 1;
				PrintMazeAroundPlr();
				if (this->plrPosY == this->colSize - 1) {
					this->playing = false;
				}
			}
			else {
				PrintMazeAroundPlr();
				cout << ANSI_RED;
				cout << "\n\nMovement Failed[YOU HIT A WALL BOZO]" << endl;
				cout << ANSI_RESET;
			}
		}
		else {
			PrintMazeAroundPlr();
			cout << ANSI_RED;
			cout << "\n\nMovement Failed[YOU CANT GET OUT OF MAZE]" << endl;
			cout << ANSI_RESET;
		}
	}
	void GoLeft() {
		if (this->plrPosX != 0) {
			if (this->v_Maze[this->plrPosY][this->plrPosX - 1] != stringMazeStats.at("Wall")) {
				this->v_Maze[this->plrPosY][this->plrPosX] = stringMazeStats.at("Space");
				this->v_Maze[this->plrPosY][this->plrPosX - 1] = stringMazeStats.at("Player");
				this->plrPosX -= 1;
				PrintMazeAroundPlr();
			}
			else {
				PrintMazeAroundPlr();
				cout << ANSI_RED;
				cout << "\n\nMovement Failed[YOU HIT A WALL BOZO]" << endl;
				cout << ANSI_RESET;
			}
		}
		else {
			PrintMazeAroundPlr();
			cout << ANSI_RED;
			cout << "\n\nMovement Failed[YOU CANT GET OUT OF MAZE]" << endl;
			cout << ANSI_RESET;
		}
	}
	void GoRight() {
		if (this->plrPosX != this->rowSize - 1) {
			if (this->v_Maze[this->plrPosY][this->plrPosX + 1] != stringMazeStats.at("Wall")) {
				this->v_Maze[this->plrPosY][this->plrPosX] = stringMazeStats.at("Space");
				this->v_Maze[this->plrPosY][this->plrPosX + 1] = stringMazeStats.at("Player");
				this->plrPosX += 1;
				PrintMazeAroundPlr();
			}
			else {
				PrintMazeAroundPlr();
				cout << ANSI_RED;
				cout << "\n\nMovement Failed[YOU HIT A WALL BOZO]" << endl;
				cout << ANSI_RESET;
			}
		}
		else {
			PrintMazeAroundPlr();
			cout << ANSI_RED;
			cout << "\n\nMovement Failed[YOU CANT GET OUT OF MAZE]" << endl;
			cout << ANSI_RESET;
		}
	}

	void SetPlrStart() {
		this->plrPosY = this->startY;
		this->plrPosX = this->startX;

		this->v_Maze[plrPosY][plrPosX] = stringMazeStats.at("Player");
	}

	void StartPlaying() {
		this->generatingMaze = false;
		this->playing = true;
		SetPlrStart();
		PrintMazeAroundPlr();
	}
};

class EasyMaze : public MazeBase {
public:
	EasyMaze() : MazeBase("Easy Maze", 1.0, 15, 15) {
		GenerateMazeAndPlay();
	};
};
class MediumMaze : public MazeBase {
public:
	MediumMaze() : MazeBase("Medium Maze", 2.0, 30, 30){
		GenerateMazeAndPlay();
	};
};
class HardMaze : public MazeBase {
public:
	HardMaze() : MazeBase("Hard Maze", 3.0, 55, 55) {
		GenerateMazeAndPlay();
	};
};
class VeryHardMaze : public MazeBase {
public:
	VeryHardMaze() : MazeBase("Very Hard Maze", 5.0, 80, 80) {
		GenerateMazeAndPlay();
	};
};
class SuperHardMaze : public MazeBase {
public:
	SuperHardMaze() : MazeBase("Super Hard Maze", 6.0, 120, 75) {
		GenerateMazeAndPlay();
	};
};
class ImpossibleMaze : public MazeBase {
public:
	ImpossibleMaze() : MazeBase("Impossible Maze", 8.0, 200, 175) {
		GenerateMazeAndPlay();
	};
};

//*********	//\\ CLASSES END //\\ ********* \\




//*********	//\\ COMMONLY USED FUNCTIONS //\\ ********* \\



//*********	//\\ COMMONLY USED FUNCTIONS END //\\ ********* \\




//*********	//\\ MAZE FUNCTIONS //\\ ********* \\


	MazeBase FindMaze(int choice) {
		try {
			if (choice == 1) return *new EasyMaze();
			if (choice == 2) return *new MediumMaze();
			if (choice == 3) return *new HardMaze();
			if (choice == 4) return *new VeryHardMaze();
			if (choice == 5) return *new SuperHardMaze();
			if (choice == 6) return *new ImpossibleMaze();

			throw "[Error Code (1)]";
		}
		catch (const char* exception) {
			cout << ANSI_RED;
			cout << "Exception Thrown: " << exception << endl;
			cout << "Handle: Default To \"Easy Maze\"" << endl;
			cout << ANSI_RESET;
			return *new EasyMaze();
		}
	}

	int StartingMessage(bool isRetrying,bool isFirst) {
		if (isRetrying == false || isFirst == true) {
			cout << ANSI_GREEN;
			cout << "\t\t-= Welcome To The Hybee Maze =-" << endl;
			cout << ANSI_BLUE;
			cout << "\n\t>> Please Chose One Of The Avaliable Mazes: " << endl;
			cout << ANSI_CYAN;
			cout << "\t\t1.Easy Maze\n\t\t2.Medium Maze\n\t\t3.Hard Maze\n\t\t4.Very Hard Maze\n\t\t5.Super Hard Maze\n\t\t6.Impossible Maze" << endl;
			cout << ANSI_BLUE;
			cout << "\t >> Choose A Number : ";
			int choice;
			cin >> choice;
			return choice;
		}
		else {
			cout << ANSI_BLUE;
			cout << "\n\t>> Please Chose One Of The Avaliable Mazes: " << endl;
			cout << ANSI_CYAN;
			cout << "\t\t1.Easy Maze\n\t\t2.Medium Maze\n\t\t3.Hard Maze\n\t\t4.Very Hard Maze\n\t\t5.Super Hard Maze\n\t\t6.Impossible Maze" << endl;
			cout << ANSI_BLUE;
			cout << "\t >> Choose A Number : ";
			int choice;
			cin >> choice;
			return choice;
		}

	}

	void EndingMessage(bool* isRetrying) {
		string end;
		cout << ANSI_GREEN;
		cout << "\n\n\n\nCONGRADULATIONS!!!!!! YOU COMPLETED THE MAZE!!!" << endl;
		cout << ANSI_RED;
		cout << "\n\nPress \"y\" to play again or press anything else and enter to leave: ";
		cin >> end;
		if (end == "y" || end == "Y") {
			*isRetrying = true;
		}
		else {
			*isRetrying = false;
			cout << ANSI_RESET;
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		}

	}

	void MovementControls(MazeBase* currMaze, bool * isRetrying) {
		cout << ANSI_RED;
		cout << "\n\nKeep In Mind While Playing \n\t\"W\" IS UP\n\t\"A\" IS LEFT\n\t\"S\" IS DOWN\n\t\"D\" IS RIGHT" << endl;
		cout << ANSI_RESET;

		while (currMaze->playing == true) {
			char movement;
			cout << "\nWhat Is Your Next Move?: ";
			cin >> movement;
			if (movement == 'W' || movement == 'w') {
				currMaze->GoUp();
			}
			else if (movement == 'A' || movement == 'a') {
				currMaze->GoLeft();
			}
			else if (movement == 'S' || movement == 's') {
				currMaze->GoDown();
			}
			else if (movement == 'D' || movement == 'd') {
				currMaze->GoRight();
			}
			else {
				ANSI_RED;
				cout << "Got Invalid Movement Key [TRY AGAIN]" << endl;
				ANSI_RESET;
			}
		}
		EndingMessage(isRetrying);
	}


//*********	//\\ MAZE FUNCTIONS END //\\ ********* \\