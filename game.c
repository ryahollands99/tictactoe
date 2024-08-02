#include <stdio.h>
#include <stdbool.h>


#define PLAYER 'X'
#define COMPUTER 'O'

char board[3][3];
unsigned int xcoord;
unsigned int ycoord;


/*Function to write title, ask and print user's name, and draw initial board */
void startGame(){

   char name[20];

   printf("****************************************************************\n");
   printf("Tic Tac Toe\n");
   printf("COMP 2131\tProgramming Assignment 1\tRyan Hollands T00605613\n");
   printf("****************************************************************\n");

   /*Get  user's name */
   printf("What is your name?\n\n");
   scanf("%s", name);
   printf("\n\n");
   printf("Hello %s ... Let's have fun !!!  You = X | Computer = O", name);
   printf("\n\n");
}

/*Draw tic tac toe board */
void drawBoard(){

   /*create 3 rows for the board */
   for (int i = 0; i < 3; i++){

      /*Create 3 columns for each row */
      for (int j = 0; j <  3; j++){
         if(board[i][j] == '\0'){  //checks if the space is null and formats accordingly
            printf(" ");
            if (j < 2) printf ("  |");
         }else{
            printf(" %c ", board[i][j]); //checks if the space has a character and formats accordingly
            if (j < 2) printf ("|");
         }
      }
      if (i < 2) printf("\n---|---|---\n");
   }
   printf("\n");
 }

/*Gets the Grid Coordinates from the Player*/
void getCoordinates(){

   /*Create an empty array of 3 characters*/
   char coordinate[3];
   for (int i =0; i < 3; i++){
      coordinate[i] = '\0';
   }

   /*Gets coordinates from Player */
   printf("Enter X,Y coordinates for your move: ");
   scanf("%s", coordinate);


   /*Convert character to integer by minus ASCII '0' and substract 1 to get index */ 
   xcoord = coordinate[0] - '0' - 1;
   ycoord = coordinate[2] - '0' - 1;

   /*Checks that the coordinates fall within the 3x3 Board. If not, reiterates getCoordinates()*/
   if (xcoord < 0 || xcoord > 2){
      printf("The coordinates are not on the board\n");
      getCoordinates();
   }


   /*Checks if the grid space is empty, if not then reiterates getCoordinates()*/
   if (board[xcoord][ycoord] != '\0'){
      printf("The space is already filled, please select another space\n");
      getCoordinates();
   }
}

/*Calls getCoordintes() and assign the Player's X to the board*/
void playerTurn(){

   getCoordinates();
   board[xcoord][ycoord] = PLAYER;
}




//Check the current status of the board and provide a score of 10 points if Computer wins, -10 points if Player wins, 0 points for a tie
int checkBoardStatus(){


   /*Check Diagonal Winner */
   if ((board[0][0] == board[1][1]) && (board[0][0] == board[2][2])){
       if (board[1][1] == COMPUTER) return +10;
       else if (board[1][1] == PLAYER) return -10;

   }

   if ((board[0][2] == board[1][1]) && (board[1][1] == board[2][0])){
       if (board[1][1] == COMPUTER) return +10;
       else if (board[1][1] == PLAYER) return -10;

   }

   /*Check the rows for a winner */
   for (int row = 0; row < 3; row++){
      if ((board [row][0] == board[row][1]) && (board[row][0] == board[row][2])){
         if (board[row][0] == COMPUTER) return +10;
	 else if (board [row][0] == PLAYER) return -10;
      }
   }

   /*Check the columns for a winner */
   for (int col = 0; col < 3; col++){
      if ((board[0][col] == board[1][col]) && (board[0][col] == board[2][col])){
         if (board[0][col] == COMPUTER) return +10;
         else if (board[0][col] == PLAYER) return -10;
      }
   }

   //if no winner
   return 0;
}

/*Checks if there any plays remaining, if not returns false*/
bool checkSquareLeft(){


   for (int row = 0; row < 3; row++){
      for (int col = 0; col < 3; col++){
         if (board[row][col] == '\0'){  //If there is any empty space then return true
            return true;
         }
      }
   }
   return false;

}


/* Implement minimax alogorithm with alpha-beta pruning to reduce the depth from 9 if a possibility is found earlier*/
int minimax(int depth, bool isMax, int alpha, int beta){

   //Get the current board score
   int score = checkBoardStatus();

   //Check if either the Computer or Player won, or if there is no squares on the grid left
   if (score == 10) return score - depth;
   if (score == -10) return score + depth;
   if (!checkSquareLeft()) return 0;


   for (int row = 0; row < 3; row++){
      for (int col = 0; col < 3; col++){

         if (board[row][col] == '\0'){
            if (isMax){  //maximize the computer score

	       int best = -100; //initialize the computer score to a low amount 
               board[row][col] = COMPUTER;

               //Recursively call minimax and select the higher score
               if (best < minimax(depth+1, !isMax, alpha, beta)){
                  best = minimax(depth+1, !isMax, alpha,beta);
               }
               if (best >= alpha){
                  alpha = best;
               }

               //return the square back to it's original state
               board[row][col] = '\0';

                 //stops the program if a possibility is found.
	       if (beta <= alpha) break;

               return best;

            } else { //minimze the player's score


               int best = 100;  //initialize the player score to a high amount
               board[row][col] = PLAYER;

               //Recursively call minimax and select the lower score
               if (best > minimax(depth+1, !isMax, alpha, beta)){
                  best = minimax(depth+1, !isMax, alpha, beta);
               }
               if (best <= beta){
                  beta = best;
               }

               //returns the square back to it's original state 
               board[row][col] = '\0';

               //stops the program if a possibility is found
               if (beta <= alpha) break;

               return best;
           }
        }
     }
   }
}

/*Finds the best move for the Computer*/
void findBestMove(){
   int bestVal = -100; //sets the computer score low 
   unsigned int bestRow = 1;
   unsigned int bestCol = 1;


   for (int row = 0; row < 3; row++){
      for (int col = 0; col < 3; col++){
        //Checks if the square is empty and assign it to the Computer
        if (board[row][col] == '\0'){
           board[row][col] = COMPUTER;

           //Gets the score from minimax()
           int moveVal = minimax(0, false, -100, 100);

           board[row][col] = '\0';

           //if the score is higher than bestVal, change the index for the row and col
           if (moveVal > bestVal){
              bestRow = row;
              bestCol = col;
              bestVal = moveVal;
           }


        }
      }
   }
   //Computer makes it's turn
   board[bestRow][bestCol] = COMPUTER;
   printf("The computer selects: %d,%d\n", bestRow+1, bestCol+1);

}




/*Check if there is a winner */
bool checkWinner(){

   //Get score and check if there are any squares left
   int score = checkBoardStatus();
   bool squareLeft = checkSquareLeft();
   if(score == -10){
      printf("\n\n*********\nYou Won!\n*********\n\n");
      return true;
   }else if (score == 10){
      printf("\n\n*******************\nThe Computer Won :(\n*******************\n\n");
      return true;

   }

   if(!squareLeft){
      printf("\n\n***********\nIt's a Tie!\n**********\n\n");
      return true;
   }

   //If there is no winner and there is squares remaining, then return false
   return false;



}


int main(){

   //Display game header and get player's name
   startGame();
   drawBoard();

   bool winner = false;

   //Keep playing until there is a winner or tie
   while (!winner){

   //player's turn and check if player won
   playerTurn();
   drawBoard();
   winner = checkWinner();
   if (winner) break;

   //Computer's turn and check if the computer won
   findBestMove();
   drawBoard();
   winner = checkWinner();
}

   drawBoard();
   return 0;

}
