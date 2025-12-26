#include <stdio.h>
#include <stdbool.h>
#define black 'B' 
#define white 'W' 
#define empty 'U' 

void intialization(char board[][26], int n);
void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol);
bool checkAllDirections(char board[][26], int n, int row, int col, char color);
bool checkAnFlip(char board[][26], int n, char playerColor);
void flipInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol);
bool computerMove(char board[][26], int n, char compColor);
bool computerMove2(char board[][26], int n, char compColor);
int checkFlipped(char board[][26], int n, int row, int col, char color);
bool moveChecker(char board[][26], int n, char color);
bool gameFull(char board[][26], int n);

int main(void) {
    int num = 0;
    char compColor;
    char playerColor;
    int difficulty = 1;
    printf("\n\nWelcome to VC's Reversi/Othello game!\n\n");
    printf("The rules follow any other reversi/othello game.\nYou will get the choice to be player black (B) or white (W). Black always goes first. \n'U' indicates an empty space on the board\n");
    printf("You will be versing a computer player whose difficulty you get to pick (level 1 or 2)\nThe board dimensions you pick must be an even number between 4 and 16\n\n\n");
    
    printf("Enter the board dimension: ");
    scanf("%d", &num);
    if (num%2 != 0 || num < 4){
        printf("\nInvalid board dimension entered!\n\n\n");
        return 0;
    }

    printf("Computer plays (B/W) : ");
    scanf(" %c", &compColor);

    if (compColor == 'W'){
        playerColor = 'B';
        printf("You are player Black (B)\n");
    } else{
        playerColor = 'W';
        printf("You are player White (W)\n");
    }

    printf("Choose computer player difficulty (1 or 2): ");
    scanf(" %d", &difficulty);
    if (difficulty != 1 && difficulty != 2){
        printf("\nInvalid difficulty entered!\n\n\n");
        return 0;
    }

    char board[26][26];
    intialization(board, num);
    printf("\nInitial Game Board:\n");
    printBoard(board, num);
    printf("\n\n");


    //if computer is black, make a move, otherwise let loop run
    if (compColor == black){
        computerMove(board, num, compColor);
        printBoard(board, num);
    }


    while (!gameFull(board, num) && (moveChecker(board, num, playerColor) || moveChecker(board, num, compColor))){
        
        //if user has no valid move pass it to computer, otherwise check if they entered a valid move w/ function
        if (!gameFull(board, num) && !moveChecker(board, num, playerColor)){
            printf("%c player has no valid move.\n", playerColor);
            } else if (checkAnFlip(board, num, playerColor) == false){
                printf("%c player wins.\n", compColor);
                //the game has ended,so end main function
                return 0;
            } else if (!gameFull(board, num)){
                printBoard(board, num);
                }
        
        //if computer has no move, say that and pass it on, otherwise it does the move
        if (!gameFull(board, num)&& !moveChecker(board, num, compColor)){
            printf("%c player has no valid move.\n", compColor);
        } else if (difficulty == 2){
            computerMove2(board, num, compColor);
            //print the board because the move was valid
            if(!gameFull(board,  num)){
                printBoard(board, num);
                } 
        } else {
            computerMove(board, num, compColor);
            //print the board because the move was valid
            if(!gameFull(board,  num)){
                printBoard(board, num);
                } 
        }

        }
    
//game has ended with no moves left, check winner and end announce winner

    int blackTiles = 0, whiteTiles = 0;
    for (int row = 0; row < num; row ++){
        for (int col = 0; col < num; col ++){
            if (board[row][col] == black){
                blackTiles = blackTiles + 1;
            } else if (board[row][col] == white){
                whiteTiles = whiteTiles + 1;
            }
        }
    }
    if (whiteTiles > blackTiles){
        printBoard(board, num);
        printf("W player wins.\n");
    } else if(blackTiles > whiteTiles){
        printBoard(board, num);
        printf("B player wins.\n");
    } else{
        printBoard(board, num);
        printf("Draw!");
    }

    return 0;
}




void intialization(char board[][26], int n){
    int dist = (n - 4)/2;

    for (int row = 0; row < n; row ++){
        for (int col = 0; col < n; col ++){
            board[row][col] = empty;
            if ((row == dist + 1 && col == dist + 1) || (row == dist + 2 && col == dist + 2)){
                board[row][col] = white;
            } if ((row == dist + 1 && col == dist + 2) || (row == dist + 2 && col == dist + 1)){
                board[row][col] = black;
            }
        }
    }
}


void printBoard(char board[][26], int n){
    char letter = 'a';

    //print top label row
    printf("  ");
    int row = 0, col = 0;
    for (int num = 0; num < n; num ++){
        printf("%c", letter + num);
    }
    printf("\n");

    //print letter space stuff
    for (row = 0; row < n; row++){
        for (col = 0; col < n; col ++){
            if (col == 0){
                printf("%c ", letter + row);
            }
            printf("%c", board[row][col]);
        }
        printf("\n");
    }
}



bool positionInBounds(int n, int row, int col){
    if (row >= n || row < 0 || col >= n || col < 0){
        return false;
    } else {
        return true;
    }
}

//check legality of a direction
bool checkLegalInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol){
    int i = 1;
    bool foundOpposite = false;
    
    while (true) {
        int stepRow = row + i * deltaRow;
        int stepCol = col + i * deltaCol;
        
        if (!positionInBounds(n, stepRow, stepCol)) {
            return false;
        }
        char current = board[stepRow][stepCol];

        if (current == empty) {
            return false; // Empty space breaks the sequence
        }
        if (current == color) {
            return foundOpposite; // Valid if at least one opposite was found
        }
        
        foundOpposite = true; // Found at least one opposite color
        i++;
    }
}
//use above function to check legality of all directions
bool checkAllDirections(char board[][26], int n, int row, int col, char color){
    for (int i = -1; i<2; i++) {
        for (int j = -1; j<2; j++){
            if ((j!= 0 || i!= 0) && checkLegalInDirection(board, n, row, col, color, i, j)) {
                return true;
            }
        }
    }
    return false;
}

bool moveChecker(char board[][26], int n, char color){
    for (int row = 0; row < n; row++){
        for (int col = 0; col < n; col ++){
            if (board[row][col] == empty && checkAllDirections(board, n, row, col, color)){
                return true;
            }
        }
    }
    return false;
}



void flipInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol) {
    int i = 1;
    int stepRow = i * deltaRow;
    int stepCol = i * deltaCol;
    while (positionInBounds(n, row, col) && board[row + stepRow][col + stepCol] != color) {
        board[row + stepRow][col + stepCol] = color;
        i++;
        stepRow = i * deltaRow;
        stepCol = i * deltaCol;
    }
}

int checkFlipped(char board[][26], int n, int row, int col, char color) {
    int flipped = 0;
    for (int deltaRow = -1; deltaRow <2; deltaRow ++) {
        for (int deltaCol = -1; deltaCol <2; deltaCol++){
            if (!(deltaRow == 0 && deltaCol == 0 ) ){
                int i = 1, counter = 0;
                while (true) {
                    int stepRow = row + i * deltaRow;
                    int stepCol = col + i * deltaCol;

                    if (!positionInBounds(n, stepRow, stepCol) || board[stepRow][stepCol] == empty){
                        counter = 0;
                        break;
                    }

                    if (board[stepRow][stepCol] == color){
                        flipped += counter;
                        break;
                    }
                    counter++;
                    i++;
                }
            }
        }
    }
    return flipped;
}



bool checkAnFlip(char board[][26], int n, char playerColor){
    //take input and organize it
    char move[3];
    printf("Enter move for colour %c (RowCol): ", playerColor);
    scanf("%s", move);
    int row = move[0] - 'a';
    int col = move[1] - 'a';


    //check and flip in each direction
    if (!positionInBounds(n, row, col) || board[row][col] != empty || !checkAllDirections(board, n, row, col, playerColor)){
        printf("Invalid move.\n");
        return false;
    } else {
        board[row][col] = playerColor;
        for (int deltaRow = -1; deltaRow <2; deltaRow ++) {
            for (int deltaCol = -1; deltaCol <2; deltaCol++){
                if (!(deltaRow == 0 && deltaCol == 0 ) && checkLegalInDirection(board, n, row, col, playerColor, deltaRow, deltaCol)){
                    flipInDirection(board, n, row, col, playerColor, deltaRow, deltaCol);
                } 
            }
        }
    }
    return true;
}




bool computerMove(char board[][26], int n, char compColor){
    int bRow = 0;
    int bCol = 0;
    int Bi = 0;
    //find best move based on criteria
    for (int row = 0; row < n; row ++){
        for (int col = 0; col < n; col ++){
            if (checkAllDirections(board, n, row, col, compColor) && board[row][col] == empty){
                int flipped = checkFlipped(board, n, row, col, compColor);
                if (Bi < flipped || (flipped == Bi && (row < bRow || (row == bRow && col < bCol)))){
                    Bi = flipped;
                    bRow = row;
                    bCol = col;
                }
            } 
        }
    }
    if (Bi == 0){
        return false;
    }

    //make the move
    board[bRow][bCol] = compColor;

    printf("Computer places %c at %c%c.\n", compColor, 'a' + bRow, 'a' + bCol);
    for (int deltaRow = -1; deltaRow <2; deltaRow ++) {
        for (int deltaCol = -1; deltaCol <2; deltaCol++){
            if (!(deltaRow == 0 && deltaCol == 0 ) && checkLegalInDirection(board, n, bRow, bCol, compColor, deltaRow, deltaCol)){
                flipInDirection(board, n, bRow, bCol, compColor, deltaRow, deltaCol);
            }
        }
    }
    return true;
}


bool gameFull(char board[][26], int n){
    for (int row = 0; row < n; row ++){
        for (int col = 0; col < n; col ++){
            if (board[row][col] == empty){
                return false;
            }
        }
    }
    return true;
}

 





//level 2 computer player move maker function:

bool computerMove2(char board[][26], int n, char compColor){
    int bRow = 0;
    int bCol = 0;
    int Bi = 99990;
    bool foundmove = false;
    //find best move based on criteria
    for (int row = 0; row < n && !foundmove; row ++){
        for (int col = 0; col < n && !foundmove; col ++){
            //check if it's a valid move
            if (checkAllDirections(board, n, row, col, compColor) && board[row][col] == empty){
                int flipped = checkFlipped(board, n, row, col, compColor);
                //check first for corners, then for outeredge move, if move exists, make it
                if (((row==0 && col == 0) || (row==0 && col == n) || (row==n && col == 0) || (row==n && col == n)) || row == 0 || col == 0){
                    foundmove = true;
                    Bi = flipped;
                    bRow = row;
                    bCol = col;
                }
                //near centre, find the move with the least flipped (playing the long game strategy)
                else if (Bi > flipped){
                    Bi = flipped;
                    bRow = row;
                    bCol = col;
                }
            } 
        }
    }
    //if no valid move was found, number of tiles flipped doens't change, return false
    if (Bi == 99990){
        return false;
    }

    //make the move
    board[bRow][bCol] = compColor;

    printf("Computer places %c at %c%c.\n", compColor, 'a' + bRow, 'a' + bCol);
    for (int deltaRow = -1; deltaRow <2; deltaRow ++) {
        for (int deltaCol = -1; deltaCol <2; deltaCol++){
            if (!(deltaRow == 0 && deltaCol == 0 ) && checkLegalInDirection(board, n, bRow, bCol, compColor, deltaRow, deltaCol)){
                flipInDirection(board, n, bRow, bCol, compColor, deltaRow, deltaCol);
            
            }  
        }
    }
    return true;
}