// MiniGames.cpp
//

#include "stdafx.h"
#include "MiniGames.h"
using namespace std;

// random integer value within given range
int random(int min, int max){
	if (min > max){ // make sure min/max aren't switched
		int temp = min;
		min = max;
		max = temp;
	}
	return(min + (rand() % (max - min + 1)));
}

// display board in console
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

// display both player and enemy boards in console
void printBoards(string board1[boardSize][boardSize], string board2[boardSize][boardSize]){
	cout << "|---|---|---|---|---|---|---|---|---|---|---|   |---|---|---|---|---|---|---|---|---|---|---|" << endl;
	cout << "|                Enemy Board                |   |               Player  Board               |" << endl;
	cout << "|---|---|---|---|---|---|---|---|---|---|---|   |---|---|---|---|---|---|---|---|---|---|---|" << endl;
	cout << "|   | A | B | C | D | E | F | G | H | I | J |   |   | A | B | C | D | E | F | G | H | I | J |" << endl;
	cout << "|---|---|---|---|---|---|---|---|---|---|---|   |---|---|---|---|---|---|---|---|---|---|---|" << endl;
	for (int i = 0; i < boardSize; i++){
		cout << "| " << i << " |";
		for (int j = 0; j < boardSize; j++){
			cout << " " + board1[i][j] + " |";
		}
		cout << "   | " << i << " |";
		for (int j = 0; j < boardSize; j++){
			cout << " " + board2[i][j] + " |";
		}
		cout << endl << "|---|---|---|---|---|---|---|---|---|---|---|   |---|---|---|---|---|---|---|---|---|---|---|" << endl;
	}
}

// clear board
void resetBoard(string board[boardSize][boardSize]){
	for (int i = 0; i < boardSize; i++){
		for (int j = 0; j < boardSize; j++){
			board[i][j] = " ";
		}
	}
}

// get user input for the column they wish to attack
int getCol(){
	string input;
	int col = -1;
	bool valid = false;
	while (!valid){
		cout << "Which column? ";
		getline(cin, input);
		col = getColumnNum(input);
		if (col < 0){ // getColumn returns -1 if no match found
			cout << "Invalid input! Try again." << endl;
		}
		else{
			valid = true;
		}
	}
	return col;
}

// get user input for the row they wish to attack
int getRow(){
	string input;
	int row = -1;
	bool valid = false;
	while (!valid){
		cout << "Which row? ";
		getline(cin, input);
		try{
			row = stoi(input);
		}
		catch (invalid_argument& e){
			// user didn't enter an int
		}

		if (0 <= row && row <= 9){
			valid = true;
		}
		else{
			cout << "Invalid input! Try again." << endl;
		}
	}
	return row;
}

// check if position on board is free for the ship being placed
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

// get user input for the row they wish to attack
int getOrientation(){
	string input;
	bool valid = false;
	while (!valid){
		cout << "Place ship horizontally or vertically? (h/v) ";
		getline(cin, input);
		if (input == "h" || input == "H"){
			valid = true;
			return 1;
		}
		else if (input == "v" || input == "V"){
			valid = true;
			return 0;
		}
		cout << "Invalid input! Try again." << endl;
	}
	return 1; // default horizontal
}

// go through each ship and place it on the board, ensuring two ships don't overlap
void setUpEnemyBoard(string board[boardSize][boardSize], ship ships[numShips]){
	for (int i = 0; i < numShips; i++){
		int length = ships[i].length;
		bool done = false;
		while (!done){ // loop in case multiple attempts needed
			int horizontal = rand() % 2; // randomize orientation. 0 = horizontal, 1 = vertical.
			int row = random(0, 9); // could be column depending on orientation (in this case we'll switch row/col)
			int col = random(0, (9 - length)); // where in the row/column we start. can't go past the end of the array.
			bool confirm = true;
			for (int j = col; j < col + length; j++){
				confirm = positionFree(horizontal, board, row, j);
				if (!confirm){
					break; // don't let confirm = false be overwritten if it occurs
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
				done = true;
			}
		}
	}
}

// go through each ship and place it on the board, ensuring two ships don't overlap
void setUpPlayerBoard(string board[boardSize][boardSize], ship ships[numShips]){
	for (int i = 0; i < numShips; i++){
		printBoard(board);
		int length = ships[i].length;
		string shipName = getShipName(ships[i].type);
		bool done = false;
		while (!done){ // loop in case multiple attempts needed
			cout << "Setting up " << shipName << " (length " << length << ")" << endl;
			int horizontal = getOrientation();
			int col = getCol();
			int row = getRow(); 
			bool confirm = true;
			if (horizontal && (col + length < boardSize)){
				// row static, col changes
				for (int j = col; j < col + length; j++){
					confirm = (board[row][j] == " "); // confirm space is empty
					if (!confirm){
						break; // don't let confirm = false be overwritten if it occurs
					}
				}
			}
			else if (row + length < boardSize){
				// col static, row changes
				for (int j = row; j < row + length; j++){
					confirm = (board[j][row] == " "); // confirm space is empty
					if (!confirm){
						break; // don't let confirm = false be overwritten if it occurs
					}
				}
			}
			else{
				confirm = false;
			}
			
			if (confirm){ // set position on board
				if (horizontal){
					for (int j = col; j < col + length; j++){
						board[row][j] = ships[i].type;
					}
				}
				else{
					for (int j = row; j < row + length; j++){
						board[j][col] = ships[i].type;
					}
				}
				done = true;
			}
			else{ // this is triggering when it shouldn't. fix it.
				cout << "Sorry, that's not a legal spot for the ship. Please try again." << endl;
			}
		}
	}
}

void setUpGame(string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]){
	string input;
	ship ships[numShips] = { ship(5, 'C'), ship(4, 'B'), ship(3, 'S'), ship(3, 'D'), ship(2, 'P') }; // yes i realize this will throw an error if numShips is changed
	bool done = false;
	while (!done){ // generate enemy board loop
		setUpEnemyBoard(enemyBoard, ships);
		printBoard(enemyBoard); // debug print
		cout << "Try another board?" << endl;
		getline(cin, input);
		if (input == "y"){
			resetBoard(enemyBoard);
		}
		else{
			done = true;
		}
	}
	setUpPlayerBoard(playerBoard, ships);
	printBoards(enemyBoard, playerBoard);
}

// setup game for testing (AI playing against itself. Don't want to worry about setting up board manually.)
void setUpGameAI(string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]){
	string input;
	ship ships[numShips] = { ship(5, 'C'), ship(4, 'B'), ship(3, 'S'), ship(3, 'D'), ship(2, 'P') }; // yes i realize this will throw an error if numShips is changed
	bool done = false;
	while (!done){ // generate enemy board loop
		setUpEnemyBoard(enemyBoard, ships);
		printBoard(enemyBoard); // debug print
		cout << "Try another board?" << endl;
		getline(cin, input);
		if (input == "y"){
			resetBoard(enemyBoard);
		}
		else{
			done = true;
		}
	}
	//reset done for fake player
	done = false;
	while (!done){ // generate fake player board loop
		setUpEnemyBoard(playerBoard, ships);
		printBoard(playerBoard); // debug print
		cout << "Try another board?" << endl;
		getline(cin, input);
		if (input == "y"){
			resetBoard(playerBoard);
		}
		else{
			done = true;
		}
	}
	printBoards(enemyBoard, playerBoard);
}

// check for hit on board
char checkHit(int r, int c, string board[boardSize][boardSize]){
	if (board[r][c] == "X" || board[r][c] == "O"){
		// this spot has already been attacked. ignore.
		return 'I'; 
	}
	else if (board[r][c] == " "){
		// miss. update.
		board[r][c] = "O";
		return 'O'; 
	}
	else { // space occupied by a letter representing a boat type. thus, a hit. yay! (or boo, if you're the player i suppose)
		// hit. update.
		board[r][c] = "X";
		return 'X';
	}
}

// AI attack. Return true if hit.
bool enemyAttack(string board[boardSize][boardSize]){ // so much repeated code ugh test for now then clean up
	// check for educated move first, if that doesn't work try random.
	// this is second stage of intelligence it tries all surrounding spots before moving on. 
	// next will be recognizing multiple hits in a row and what to do with them
	char hit = ' ';
	for (int r = 0; r < boardSize; r++){ //okay first step is to get the "lay of the land" (remembering AI can't "see" player ships)
		for (int c = 0; c < boardSize; c++){
			if (board[r][c] == "X"){ //we have a hit! check surrounding spaces
				// if r > 0, check left [r - 1, c]
				if (r > 0){
					hit = checkHit(r - 1, c, board);
				}
				// if r < boardSize - 1, check right [r + 1, c]
				if (r < boardSize - 1 && (hit == ' ' || hit == 'I')){
					hit = checkHit(r + 1, c, board);
				}
				// if c > 0, check down [r, c - 1]
				if (c > 0 && (hit == ' ' || hit == 'I')){
					hit = checkHit(r, c - 1, board);
				}
				// if c < boardSize - 1, check up [r, c + 1]
				if (c < boardSize - 1 && (hit == ' ' || hit == 'I')){
					hit = checkHit(r, c + 1, board);
				}

				if (hit == 'X'){
					return true;
				}
				if (hit == 'O'){
					return false;
				}
			}
		}
	}
	// if we're still here then no intelligent guesses left
	bool nohit = true;
	while (nohit){
		int row = random(0, boardSize - 1);
		int col = random(0, boardSize - 1);
		hit = checkHit(row, col, board);
		if (hit == 'X' || hit == 'O'){
			nohit = false;
		}
	}

	if (hit == 'X'){
		return true;
	}
	if (hit == 'O'){
		return false;
	}
	return false; //should never hit this (should not exit while without X or O value) but just to be safe
}

// allow user to attack until they either run out of moves or destroy all ships
void playGame(string attackBoard[boardSize][boardSize], string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]){
	string input;
	int maxHits = 17; // this also needs updating if the ships change
	int hits = 0;
	int AI_hits = 0;
	string winner = "";
	bool playing = true;
	while (playing){ // play loop
		printBoards(attackBoard, playerBoard);
		cout << "Your move!" << endl;
		int col = getCol();
		int row = getRow();
		bool playerChoseValid = true; // if player accidentally chooses somewhere they've already attacked, want to give them another chance without AI getting an extra move.
		if (attackBoard[row][col] == " "){
			if (enemyBoard[row][col] != " "){
				attackBoard[row][col] = "X";
				hits += 1;
				if (hits == maxHits){
					winner = "Player";
					playing = false;
					break;
				}
			}
			else{
				attackBoard[row][col] = "O";
			}
		}
		else{
			cout << "You've already tried that spot! Try again." << endl; 
			playerChoseValid = false;
		}
		if (playerChoseValid){
			bool enemyHit = enemyAttack(playerBoard);
			if (enemyHit){
				AI_hits += 1;
				if (AI_hits == maxHits){
					winner = "AI";
					playing = false;
					break;
				}
			}
			cout << hits << " vs " << AI_hits << endl;
		}
	}
	cout << "Game Over! " << winner << " Wins!" << endl;
	printBoards(attackBoard, playerBoard);
}

// have AI play against another AI (well, same AI but with a different board to attack) for testing purposes
void playGameAI(string enemyBoard[boardSize][boardSize], string playerBoard[boardSize][boardSize]){
	string input;
	int maxHits = 17; // this also needs updating if the ships change
	int hits = 0;
	int AI_hits = 0;
	string winner = "";
	bool playing = true;
	while (playing){ // play loop
		cout << endl;
		printBoards(enemyBoard, playerBoard);
		bool fakePlayerHit = enemyAttack(enemyBoard);
		if (fakePlayerHit){
			hits += 1;
			if (hits == maxHits){
				winner = "Player";
				playing = false;
				break;
			}
		}
		bool enemyHit = enemyAttack(playerBoard);
		if (enemyHit){
			AI_hits += 1;
			if (AI_hits == maxHits){
				winner = "AI";
				playing = false;
				break;
			}
		}
		cout << hits << " vs " << AI_hits << endl;
	}
	cout << "Game Over! " << winner << " Wins!" << endl;
	printBoards(enemyBoard, playerBoard);
}


int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Would you like to play? (y/n)" << endl;
	string input;
	getline(cin, input);
	bool continueGame = true;
	while (continueGame){ // main game loop
		if (input == "y"){
			srand(time(NULL)); // randomize the seed so we don't get the same board every time
			string enemyBoard[boardSize][boardSize]; 
			resetBoard(enemyBoard);
			string attackBoard[boardSize][boardSize]; // this board is for printing to the user (so user won't see the enemy ship locations)
			resetBoard(attackBoard);
			string playerBoard[boardSize][boardSize];
			resetBoard(playerBoard);
			cout << "Watch or Play (Default is Watch)? (w/p)" << endl;
			getline(cin, input);
			if (input == "p"){
				setUpGame(enemyBoard, playerBoard);
				cout << "Time to play!" << endl;
				playGame(attackBoard, enemyBoard, playerBoard);
				cout << "Would you like to play again?" << endl;
				getline(cin, input);
			}
			else{
				setUpGameAI(enemyBoard, playerBoard);
				cout << "Time to play!" << endl;
				playGameAI(enemyBoard, playerBoard);
				cout << "Would you like to play again?" << endl;
				getline(cin, input);
			}
		}
		else {
			continueGame = false;
		}
	}
	return 0;
}