// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <time.h>
#include <iostream>
using namespace std;
#define boardSize 10
#define numShips 5

class ship{ //simple class to set up the ships for the game
public:
	int length;
	char type;

	ship(int l, char t){
		length = l;
		type = t;
	}
};

//random integer value within given range
int random(int min, int max){
	if (min > max){ // make sure min/max aren't switched
		int temp = min;
		min = max;
		max = temp;
	}
	return(min + (rand() % (max - min + 1)));
}

//display board in console
void printBoard(string board[boardSize][boardSize]){
	cout << "|---|---|---|---|---|---|---|---|---|---|---|" << endl;
	cout << "|   | A | B | C | D | E | F | G | H | I | J |" << endl;
	cout << "|---|---|---|---|---|---|---|---|---|---|---|" << endl;
	for (int i = 0; i < boardSize; i++){
		cout << "| " << i << " |";
		for (int j = 0; j < boardSize; j++){
			cout << " " + board[i][j] + " |";
		}
		cout << endl << "|---|---|---|---|---|---|---|---|---|---|---|" << endl;
	}
}

//clear board
void resetBoard(string board[boardSize][boardSize]){
	for (int i = 0; i < boardSize; i++){
		for (int j = 0; j < boardSize; j++){
			board[i][j] = " ";
		}
	}
}

//get column number based on letter input
int getColumn(string letter){
	if (letter.length() == 1){
		letter[0] = toupper(letter[0]); //capitalize. if the string is longer than 1 than it won't match anyways.
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
int getCol(){
	string input;
	int col = -1;
	while (true){
		cout << "Which column? ";
		getline(cin, input);
		col = getColumn(input);
		if (col < 0){ //getColumn returns -1 if no match found
			cout << "Invalid input! Try again." << endl;
		}
		else{
			break;
		}
	}
	return col;
}

// get user input for the row they wish to attack
int getRow(){
	string input;
	int row = -1;
	while (true){
		cout << "Which row? ";
		getline(cin, input);
		try{
			row = stoi(input);
			if (0 <= row && row <= 9){
				break;
			}
		}
		catch (invalid_argument& e){
			// user didn't enter an int
		}
		cout << "Invalid input! Try again." << endl;
	}
	return row;
}

//check if position on board is free for the ship
bool positionFree(int& horizontal, string board[boardSize][boardSize], int& row, int& col){
	if (horizontal){
		if (board[row][col] != " "){
			return false;
		}
	}
	else{
		if (board[col][row] != " "){
			return false;
		}
	}
	return true;
}

//go through each ship and place it on the board, ensuring two ships don't overlap
void setUpBoard(string board[boardSize][boardSize], ship ships[numShips]){
	for (int i = 0; i < numShips; i++){
		int length = ships[i].length;
		while (true){ // loop in case multiple attempts needed
			int horizontal = rand() % 2; //randomize orientation. 0 = horizontal, 1 = vertical.
			int row = random(0, 9); //could be column depending on orientation (in this case we'll switch row/col)
			int col = random(0, (9 - length)); //where in the row/column we start. can't go past the end of the array.
			bool confirm = true;
			for (int j = col; j < col + length; j++){
				confirm = positionFree(horizontal, board, row, j);
				if (!confirm){
					break; //don't let confirm = false be overwritten if it occurs
				}
			}
			if (confirm){ //set position on board
				for (int j = col; j < col + length; j++){
					if (horizontal){
						board[row][j] = ships[i].type;
					}
					else{
						board[j][row] = ships[i].type;
					}
				}
				break;
			}
		}
	}
}

void setUpGame(string attackBoard[boardSize][boardSize], string enemyBoard[boardSize][boardSize]){
	string input;
	ship ships[numShips] = { ship(5, 'C'), ship(4, 'B'), ship(3, 'S'), ship(3, 'D'), ship(2, 'P') }; //yes i realize this will throw an error if numShips is changed
	while (true){ //generate enemy board loop
		setUpBoard(enemyBoard, ships);
		printBoard(enemyBoard); //debug print
		cout << "Try another board?" << endl;
		getline(cin, input);
		if (input == "y"){
			resetBoard(enemyBoard);
		}
		else{
			break;
		}
	}
}

// allow user to attack until they either run out of moves or destroy all ships
void playGame(string attackBoard[boardSize][boardSize], string enemyBoard[boardSize][boardSize]){
	string input;
	int maxMoves = 50;
	int moves = 0;
	int maxHits = 17; // this also needs updating if the ships change
	int hits = 0;
	while (true){ //play loop
		printBoard(attackBoard);
		cout << "Your move!" << endl;
		int col = getCol();
		int row = getRow();
		if (attackBoard[row][col] == " "){
			if (enemyBoard[row][col] != " "){
				attackBoard[row][col] = "X";
				hits += 1;
				if (hits == maxHits){
					break;
				}
			}
			else{
				attackBoard[row][col] = "O";
			}
			moves += 1;
			if (moves == maxMoves){
				cout << "Sorry! You're out of moves! Game Over. :(" << endl;
				break;
			}
		}
		else{
			cout << "You've already tried that spot!" << endl;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Would you like to play? (y/n)" << endl;
	string input;
	getline(cin, input);
	while (true){ //main game loop
		if (input == "y"){
			srand(time(NULL)); // randomize the seed so we don't get the same board every time
			string enemyBoard[boardSize][boardSize];
			resetBoard(enemyBoard);
			string attackBoard[boardSize][boardSize];
			resetBoard(attackBoard);
			setUpGame(attackBoard, enemyBoard);
			cout << "Time to play!" << endl;
			playGame(attackBoard, enemyBoard);
			cout << "Would you like to play again?" << endl;
			getline(cin, input);
		}
		else {
			break;
		}
	}
	cin.ignore();
	return 0;
}