#ifndef SUDOKU_H
#define SUDOKU_H

#define SUDOKU_SUCCESS         0
#define SUDOKU_NO_FILE_ERROR   1
#define SUDOKU_UNKNOWN_ERROR   2
#define SUDOKU_CONTRADICTION   3
#define SUDOKU_ILLEGAL         4
#define SUDOKU_NO_SOLUTION     5
#define SUDOKU_BAD_FORMAT      6
#define SUDOKU_SOLVED          7
#define SUDOKU_ALLOC_FAILED    8

/*
  This file describes the interface for a Sudoku First-class Abstract
  Data Type.  
 */

typedef struct sudoku Sudoku;

/* 
   Sudoku_create: Allocate a new Sudoku structure using malloc and save
   it in a global variable. You may want to initialize the game
   structure at this point.
   
   Return values:
      SUDOKU_SUCCESS: allocated successfully
      SUDOKU_ALLOC_FAILED: failed to allocate memory for the game
 */
int Sudoku_create (void);

/* 
   Sudoku_destroy: destroy Sudoku structure.  Deallocate any memory
   you allocated.
 */

void Sudoku_destroy (void);

/*
  Sudoku_loadfile: Initialize the game to the board held in filename. The
  current game, if any, is discarded.

  The structure of the file must be as follows:
    - A digit represents a value from 1-9 and a - represents an empty game position.
    - There will be 9 lines in the file. Each line corresponds to one
      row of the Sudoku puzzle
    - Each line contains 9 digits/dashes seperated by a whitespace character. Each
      digit/dash falls into one of the 9 columns. 
    - Read in the file and populate your structure.

  Return the following error codes:
     SUDOKU_SUCCESS : file read successfully
     SUDOKU_NO_FILE_ERROR : no file found or cannot open file
     SUDOKU_BAD_FORMAT : file contents in invalid format:
           - fewer than 9 digits/dashes on a line
           - fewer than 9 lines in the file
	   - unknown character
     SUDOKU_UNKNOWN_ERROR : any other error
 */
int Sudoku_loadfile (const char *filename);

/*
  Sudoku_savefile: Save the current game to filename. 

  The structure of the file must be compatible with Sudoku_loadfile.

  Return the following error codes:
     SUDOKU_SUCCESS : file read successfully
     SUDOKU_NO_FILE_ERROR : no file found or cannot open file
     SUDOKU_UNKNOWN_ERROR : any other error
 */

int Sudoku_savefile (const char *filename);

/*
  Sudoku_play: start playing the game.  In particular, you must
  take note of the current state of the grid so that you can restore it
  to this position when Sudoku_reset is called.
 */
void Sudoku_play (void);

/*
  Sudoku_set_square: set the square at position (col, row) to value
  digit. 

  Return values:
     SUDOKU_SUCCESS : square set successfully
     SUDOKU_CONTRADICTION : square is update, but it leads to a contradiction.
        - You do not need to solve the puzzle, just look at current values
	  and see if a contradiction already exists.
     SUDOKU_ILLEGAL : square is part of the original game and cannot be changed.
 */
int Sudoku_set_square (char col, int row, int digit);

/*
  Sudoku_clear_square: reset the square at position (col, row) to blank. 

  Return values:
     SUDOKU_SUCCESS : square cleared successfully
     SUDOKU_ILLEGAL : square is part of the original game and cannot be changed.
 */
int Sudoku_clear_square (char col, int row);

/*
  Sudoku_give_hint: print a hint to the player to help them
  make progress. Print the solution for one square if you can
  determine one, in the following form: "Look at A1.\n".  Otherwise,
  if no hint can be determined, print "Sorry, I'm clueless.\n" 

  Return value:
     SUDOKU_SUCCESS : hint supplied
     SUDOKU_NO_SOLUTION : no hint because solver could not find one.
     SUDOKU_CONTRADICTION : no hint because contradiction found.  
     SUDOKU_SOLVED : Sudoku is already solved   
*/
int Sudoku_give_hint (void);

/*
  Sudoku_solve: find a solution for the Sodoku puzzle using the 
  algorithm in the specification document.
  Return the following error codes:
     SUDOKU_SUCCESS : solution found
     SUDOKU_NO_SOLUTION : no solution found using solver
     SUDOKU_CONTRADICTION : contradiction found
     SUDOKU_SOLVED : Sudoku is already solved
*/
int Sudoku_solve (void);

/*
  Sudoku_reset_game: reset the board to the beginning of play.
 */
void Sudoku_reset_game (void);

/*
  Print the Sodoku game to the screen.  You must print a header
  row labeled from A to I.  You must print a leading digit for
  each row.
 */
void Sudoku_print (void);

#endif /* SUDOKU_H */
