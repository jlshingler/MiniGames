// MiniGames.h
//

#include "stdafx.h"
#include <string>
#include <time.h>
#include <iostream>
using namespace std;
#define boardSize 10
#define numShips 5

class ship{ // simple class to set up the ships for the game
public:
	int length;
	char type;

	ship(int l, char t){
		length = l;
		type = t;
	}
};

// random integer value within given range
int random(int min, int max);

// display board in console
void printBoard(string board[boardSize][boardSize]);

// display both player and enemy boards in console
void printBoards(string board1[boardSize][boardSize], string board2[boardSize][boardSize]);

// clear board
void resetBoard(string board[boardSize][boardSize]);

// translate ship type char to string of ship name
string getShipName(char type){
	switch (type){
	case 'C':
		return "Aircraft Carrier";
	case 'B':
		return "Battleship";
	case 'S':
		return "Submarine";
	case 'D':
		return "Destroyer";
	case 'P':
		return "Patrol Boat";
	default:
		return "Invalid Ship Type";
	}
}

// get column number based on letter input
int getColumnNum(string letter){
	if (letter.length() == 1){
		letter[0] = toupper(letter[0]); // capitalize. if the string is longer than 1 than it won't match anyways.
		string columns[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };
		for (int i = 0; i < boardSize; i++){
			if (columns[i] == letter){
				return i;
			}
		}
	}
	return -1;
}

// get user input for the column they wish to attack
int getCol();

// get user input for the row they wish to attack
int getRow();

// check if position on board is free for the ship being placed
bool positionFree(int& horizontal, string board[boardSize][boardSize], int& row, int& col);

// get user input for the row they wish to attack
int getOrientation();

// go through each ship and place it on the board, ensuring two ships don't overlap
void setUpEnemyBoard(string board[boardSize][boardSize], ship ships[numShips]);

// go through each ship and place it on the board, ensuring two ships don't overlap
void setUpPlayerBoard(string board[boardSize][boardSize], ship ships[numShips]);

void setUpGame(string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]);

// setup game for testing (AI playing against itself. Don't want to worry about setting up board manually.)
void setUpGameAI(string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]);

// check for hit on board
char checkHit(int r, int c, string board[boardSize][boardSize]);

// AI attack. Return true if hit.
bool enemyAttack(string board[boardSize][boardSize]);

// allow user to attack until they either run out of moves or destroy all ships
void playGame(string attackBoard[boardSize][boardSize], string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]);

// have AI play against another AI (well, same AI but with a different board to attack) for testing purposes
void playGameAI(string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]);