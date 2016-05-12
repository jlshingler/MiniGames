// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <time.h>
#include <iostream>
using namespace std;
#define boardSize 10
#define numShips 5

class ship{
public:
	int length;
	int orientation;
	char type;

	ship(int l, char t){
		length = l;
		type = t;
		orientation = 0; // 0 = horizontal, 1 = vertical, default to horizontal
	}
};

int random(int min, int max){
	if (min > max){ // make sure min/max aren't switched
		int temp = min;
		min = max;
		max = temp;
	}
	return(min + (rand() % (max - min + 1)));
}

int getColumn(string letter){
	string columns[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };
	for (int i = 0; i < sizeof(columns); i++){
		if (columns[i] == letter){
			return i;
		}
	}
	return 0;
}

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

void resetBoard(string board[boardSize][boardSize]){
	for (int i = 0; i < boardSize; i++){
		for (int j = 0; j < boardSize; j++){
			board[i][j] = " ";
		}
	}
}

void setUpBoard(string board[boardSize][boardSize], ship ships[numShips]){
	for (int i = 0; i < numShips; i++){
		int length = ships[i].length;
		while (true){
			ships[i].orientation = rand() % 2; //randomize orientation
			int row = random(0, 9); //could be column depending on orientation but this name makes most sense to me
			int start = random(0, (9 - length)); //where in the row/column we start. can't go past the end of the array.
			bool confirm = true;
			if (ships[i].orientation){ // orientation is 1 = vertical (row = column)
				for (int j = start; j < start + length; j++){
					if (board[j][row] != " "){
						confirm = false;
					}
				}
			}
			else{ // orientation is 0 = horizontal (row = row)
				for (int j = start; j < start + length; j++){
					if (board[row][j] != " "){
						confirm = false;
					}
				}
			}
			if (confirm){ //set position on board
				if (ships[i].orientation){
					for (int j = start; j < start + length; j++){
						board[j][row] = ships[i].type;
					}
				}
				else{
					for (int j = start; j < start + length; j++){
						board[row][j] = ships[i].type;
					}
				}
				break;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Would you like to play?" << endl;
	string input;
	getline(cin, input);
	while (true){ //main game loop
		if (input == "y"){
			srand(time(NULL)); // randomize the seed so we don't get the same board every time
			string enemyBoard[boardSize][boardSize];
			string attackBoard[boardSize][boardSize];
			resetBoard(enemyBoard);
			resetBoard(attackBoard);
			ship ships[numShips] = { ship(5, 'C'), ship(4, 'B'), ship(3, 'S'), ship(3, 'D'), ship(2, 'P') }; //yes i realize this will throw an error if numShips is changed
			int maxHits = 17; // this also needs updating if the ships change
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
			cout << "Time to play!" << endl;
			int maxMoves = 50;
			int moves = 0;
			int hits = 0;
			while (true){ //play loop
				printBoard(attackBoard);
				cout << "Your move!" << endl;
				cout << "Which column? ";
				getline(cin, input);
				int col = getColumn(input); // yes i know there's no error handling don't worry about it yet
				cout << endl << "Which row? ";
				getline(cin, input);
				int row = stoi(input); // still no error handling shhhhhhhhhhhhhh
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