/*----------------------------------------------------------------------
   Assignment: Program 4
   Date: 4/20/10
   
   Programmer: Michael Oglesby 
   Course: ECE 209 - 001 (Problem session section 402)              
   Professor: Dr. James Tuck
   
----------------------------------------------------------------------*/

#include "sudoku.h"
#include "stdlib.h"
#include "stdio.h"


/* struct representing one square in the sudoku game */
struct sudoku_square {
	int value;	/* value of the square */
	int solution_set;	/* bit-vector solution set for the square taking for format: 9876543210 */
};

/* struct representing an entire sudoku game */
struct sudoku {  
	struct sudoku_square grid[81];	/* current sudoku grid (array of 81 squares...indices 0-8 represent the first row, and so on */
	struct sudoku_square backup[81];	/* backup/saved sudoku grid */
};

Sudoku *game;	/* global variable representing the sudoku game */


/*======================================================================*/
/*======================================================================*/


/* IMPLEMENT SUDOKO INTERFACE */

/* 
   Sudoku_create: Allocate a new Sudoku structure using malloc and save
   it in a global variable. You may want to initialize the game
   structure at this point.
   
   Return values:
      SUDOKU_SUCCESS: allocated successfully
      SUDOKU_ALLOC_FAILED: failed to allocate memory for the game
 */
int Sudoku_create (void)
{
	game = (Sudoku*) malloc( sizeof(Sudoku) );	/* Allocate sudoku structure */

	if (!game)
		return SUDOKU_ALLOC_FAILED;

	return SUDOKU_SUCCESS;
}

/* 
   Sudoku_destroy: destroy Sudoku structure.  Deallocate any memory
   you allocated.
 */

void Sudoku_destroy(void)
{
	free(game);
}

/*
  Sudoku_loadfile: Initialize the game to the board held in filename. The
  current game, if any, is discarded.

  The structure of the file must be as follows:
    - A digit represents a value from 1-9 and a dash represents an empty game position.
    - There will be 9 lines in the file. Each line corresponds to one
      row of the Sudoku puzzle
    - Each line contains 9 digits/dashes seperated by a whitespace character. Each
      digit/dash falls into one of the 9 columns. 
    - Read in the file and populate your structure.

  Return the following error codes:
     SUDOKU_SUCCESS : file read successfully
     SUDOKU_NO_FILE_ERROR : no file found or cannot open file
     SUDOKU_BAD_FORMAT : file contents in invalid format
     SUDOKU_UNKNOWN_ERROR : any other error
 */
int Sudoku_loadfile(const char *filename)
{
  	FILE *fptr = fopen(filename, "r");
	char temp;
	int error = 0;	
	int i = 0;

	/* if file cannot be opened, return SUDOKU_NO_FILE_ERROR */
	if (!fptr)
		return SUDOKU_NO_FILE_ERROR;
	
	/* populate each square of sudoku game from file source */
	while ( ( temp = fgetc(fptr) ) != EOF )
	{
		if (temp >= '1' && temp <= '9')
		{
			(game->grid[i]).value = temp - 48;
			i++;
		}
		else if (temp == '-')
		{
			(game->grid[i]).value = 0;
			i++;
		}
		else if (temp != ' ' && temp != '\n')
			error = 1;
	}

	fclose(fptr);
	
	/* if file contents in invalid format, return SUDOKU_BAD_FORMAT */
	if (i < 81 || error)
		return SUDOKU_BAD_FORMAT;

	if (i > 81)
		return SUDOKU_UNKNOWN_ERROR;
  
	/* printf("Load game from %s\n",filename); */

	return SUDOKU_SUCCESS;
}
/*
  Sudoku_savefile: Save the current game to filename. 

  The structure of the file must be compatible with Sudoku_loadfile.

  Return the following error codes:
     SUDOKU_SUCCESS : file read successfully
     SUDOKU_NO_FILE_ERROR : no file found or cannot open file
     SUDOKU_UNKNOWN_ERROR : any other error
 */

int Sudoku_savefile(const char *filename)
{
 	FILE *fptr = fopen(filename, "w");
	int i;

	/* if file cannot be opened, return SUDOKU_FILE_ERROR */
	if (!fptr)
		return SUDOKU_NO_FILE_ERROR;

	/* save each square to file in appropriate position */
	for (i = 0; i < 81; i++)
	{
		if ( (i + 1) % 9 )
		{
			if ((game->grid[i]).value)
				fprintf(fptr, "%d ", (game->grid[i]).value );
			else
				fprintf(fptr, "- ");
		}
		else
		{
			if ((game->grid[i]).value)
				fprintf(fptr, "%d\n", (game->grid[i]).value );
			else
				fprintf(fptr, "-\n");
		}
	}

	fclose(fptr);	

  	/* printf("Store game to %s\n",filename); */

  	return SUDOKU_SUCCESS;
}

/*
  Sudoku_play: start playing the game.  In particular, you must
  take note of the current state of the grid so that you can restore it
  to this position when Sudoku_reset is called.
 */
void Sudoku_play(void)
{
 	int i;

	/* save current state of game */
	for (i = 0; i < 81; i++)
	{
		(game->backup[i]).value = (game->grid[i]).value;
		(game->backup[i]).solution_set = (game->grid[i]).solution_set;
	}	

 	/* printf("Begin play.\n"); */
}

/*
  Sudoku_set_square: set the square at position (col, row) to value
  digit. 

  Return values:
     SUDOKU_SUCCESS : square set successfully
     SUDOKU_CONTRADICTION : square set but it leads to a contradiction.
        - You do not need to solve the puzzle, just look at current values
	  and see if a contradiction already exists.
     SUDOKU_ILLEGAL : square is part of the original game and cannot be changed.
 */
int Sudoku_set_square(char col, int row, int digit)
{
	int i = (row * 9) - 9 + (col - 65);
	int j;
	int k;

	/* if square is part of the original game, return SUDOKU_ILLEGAL */
	if ( (game->backup[i]).value )
		return SUDOKU_ILLEGAL;

	(game->grid[i]).value = digit;	/* set the square to desired value */

	/* reduce possible solutions based on rows */
	for (j = i - (i % 9); j < i - (i % 9) + 9; j++)
	{
		switch ( (game->grid[j]).value )
		{
			case 0:	break;
			case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
			case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
			case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
			case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
			case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
			case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
			case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
			case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
			case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
		}	
	}

	/* reduce possible solutions based on columns */
	for (j = i % 9; j < 81; j+=9)
	{
		switch ( (game->grid[j]).value )
		{
			case 0:	break;
			case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
			case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
			case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
			case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
			case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
			case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
			case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
			case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
			case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
		}
	}

	/* reduce possible solutions based on blocks */

	j = i - (i % 27) + ( (i % 9) / 3 ) * 3;
	k = j;
	while (j < k + 3)
	{
		switch ( (game->grid[j]).value )
		{
			case 0:	break;
			case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
			case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
			case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
			case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
			case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
			case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
			case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
			case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
			case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
		}
		j++;				
	}

	j+=6;
	k = j;
	while (j < k + 3)
	{
		switch ( (game->grid[j]).value )
		{
			case 0:	break;
			case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
			case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
			case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
			case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
			case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
			case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
			case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
			case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
			case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
		}
		j++;				
	}

	j+=6;
	k = j;
	while (j < k + 3)
	{
		switch ( (game->grid[j]).value )
		{
			case 0:	break;
			case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
			case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
			case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
			case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
			case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
			case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
			case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
			case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
			case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
		}
		j++;				
	}

	/* if the solution set does not match the value, return SUDOKU_CONTRADICTION */
	switch ( (game->grid[i]).value )
	{
		case 1: 
			if ( !( (game->grid[i]).solution_set & 0x2 ) )
				return SUDOKU_CONTRADICTION; break;
		case 2: 
			if ( !( (game->grid[i]).solution_set & 0x4 ) )
				return SUDOKU_CONTRADICTION; break;
		case 3: 
			if ( !( (game->grid[i]).solution_set & 0x8 ) )
				return SUDOKU_CONTRADICTION; break;
		case 4: 
			if ( !( (game->grid[i]).solution_set & 0x10 ) )
				return SUDOKU_CONTRADICTION; break;
		case 5: 
			if ( !( (game->grid[i]).solution_set & 0x20 ) )
				return SUDOKU_CONTRADICTION; break;
		case 6: 
			if ( !( (game->grid[i]).solution_set & 0x40 ) )
				return SUDOKU_CONTRADICTION; break;
		case 7: 
			if ( !( (game->grid[i]).solution_set & 0x80 ) )
				return SUDOKU_CONTRADICTION; break;
		case 8: 
			if ( !( (game->grid[i]).solution_set & 0x100 ) )
				return SUDOKU_CONTRADICTION; break;
		case 9: 
			if ( !( (game->grid[i]).solution_set & 0x200 ) )
				return SUDOKU_CONTRADICTION; break;
	}	

	/* printf("Set square %c%d to %d.\n",col,row,digit); */
	return SUDOKU_SUCCESS;
}

/*
  Sudoku_clear_square: reset the square at position (col, row) to blank. 

  Return values:
     SUDOKU_SUCCESS : square cleared successfully
     SUDOKU_ILLEGAL : square is part of the original game and cannot be changed.
 */
int Sudoku_clear_square(char col, int row)
{
	int i = (row * 9) - 9 + (col - 65);

	/* if square is part of the original game, return SUDOKU_ILLEGAL */
	if ( (game->backup[i]).value )
		return SUDOKU_ILLEGAL;

	(game->grid[i]).value = 0;	/* clear square */

	/* printf("Clear square %c%d.\n",col,row); */
	return SUDOKU_SUCCESS;
}

/*
  Sudoku_give_hint: print a hint to the player to help them
  make progress. Print the solution for one square if you can
  determine one, in the following form: "Try A1\n".  Otherwise,
  if no hint can be determined, print "Sorry, I'm clueless.\n" 

  Return value:
     SUDOKU_SUCCESS : hint supplied
     SUDOKU_NO_SOLUTION : no hint because solver could not find one.
     SUDOKU_CONTRADICTION : no hint because contradiction found.     
*/
int Sudoku_give_hint(void)
{
	int found = -1;	/* flag variable indicating that we have found a square for which a solution exists */
	int i;	/* index variable */
	int j;	/* index variable */
	int k;	/* index variable */
	int contradiction = 0;	/* flag variable indicating that a contradiction occurred */

	/* solve algorithm adapted from pseudocode taken from program spec */

	/* inidialize solution set for each square */
	for (i = 0; i < 81; i++)
	{
		switch ( (game->grid[i]).value )
		{
			case 0:	(game->grid[i]).solution_set = 0x3FE; break;
			case 1: (game->grid[i]).solution_set = 0x2; break;
			case 2: (game->grid[i]).solution_set = 0x4; break;
			case 3: (game->grid[i]).solution_set = 0x8; break;
			case 4: (game->grid[i]).solution_set = 0x10; break;
			case 5: (game->grid[i]).solution_set = 0x20; break;
			case 6: (game->grid[i]).solution_set = 0x40; break;
			case 7: (game->grid[i]).solution_set = 0x80; break;
			case 8: (game->grid[i]).solution_set = 0x100; break;
			case 9: (game->grid[i]).solution_set = 0x200; break;
		}
	}

	for (i = 0; i < 81; i++)
	{
		/* reduce possible solutions based on rows */
		for (j = i - (i % 9); j < i - (i % 9) + 9; j++)
		{
			switch ( (game->grid[j]).value )
			{
				case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
				case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
				case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
				case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
				case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
				case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
				case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
				case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
				case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
			}	
		}

		/* reduce possible solutions based on columns */
		for (j = i % 9; j < 81; j+=9)
		{
			switch ( (game->grid[j]).value )
			{
				case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
				case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
				case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
				case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
				case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
				case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
				case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
				case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
				case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
			}
		}

		/* reduce possible solutions based on blocks */
			
		j = i - (i % 27) + ( (i % 9) / 3 ) * 3;
		k = j + 3;
		while (j < k)
		{
			switch ( (game->grid[j]).value )
			{
				case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
				case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
				case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
				case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
				case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
				case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
				case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
				case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
				case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
			}
			j++;				
		}

		j+=6;
		k = j + 3;
		while (j < k)
		{
			switch ( (game->grid[j]).value )
			{
				case 0:	break;
				case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
				case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
				case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
				case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
				case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
				case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
				case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
				case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
				case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
			}
			j++;				
		}

		j+=6;
		k = j + 3;
		while (j < k)
		{
			switch ( (game->grid[j]).value )
			{
				case 0:	break;
				case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
				case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
				case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
				case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
				case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
				case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
				case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
				case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
				case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
			}
			j++;				
		}
	}

	for (i = 0; i < 81; i++)
	{
		/* for each square that is blank */
		if ( (game->grid[i]).value == 0 )
		{
			/* if there is only one valid solution, set the square to that solution and set found to the index of the square */
			switch ( (game->grid[i]).solution_set )
			{
				case 0x2: (game->grid[i]).value = 1; found = i; break;
				case 0x4: (game->grid[i]).value = 2; found = i; break;
				case 0x8: (game->grid[i]).value = 3; found = i; break;
				case 0x10: (game->grid[i]).value = 4; found = i; break;
				case 0x20: (game->grid[i]).value = 5; found = i; break;
				case 0x40: (game->grid[i]).value = 6; found = i; break;
				case 0x80: (game->grid[i]).value = 7; found = i; break;
				case 0x100: (game->grid[i]).value = 8; found = i; break;
				case 0x200: (game->grid[i]).value = 9; found = i; break;
			}		
		}

		/* if we found a solution, break out of loop...this square (index = found) will be given as a hint */
		if (found != -1)
			break;	
	}

	/* if the square is not blank */
	if ( (game->grid[found]).value )
	{
		/* if the solution set does not match the value, contradiction = true */
		switch ( (game->grid[found]).value )
		{
			case 1: 
				if ( (game->grid[found]).solution_set != 0x2 )
					contradiction = 1; 
				break;
			case 2: 
				if ( (game->grid[found]).solution_set != 0x4 )
					contradiction = 1; 
				break;
			case 3: 
				if ( (game->grid[found]).solution_set != 0x8 )
					contradiction = 1; 
				break;
			case 4: 
				if ( (game->grid[found]).solution_set != 0x10 )
					contradiction = 1; 
				break;
			case 5: 
				if ( (game->grid[found]).solution_set != 0x20 )
					contradiction = 1; 
				break;
			case 6: 
				if ( (game->grid[found]).solution_set != 0x40 )
					contradiction = 1; 
				break;
			case 7: 
				if ( (game->grid[found]).solution_set != 0x80 )
					contradiction = 1; 
				break;
			case 8: 
				if ( (game->grid[found]).solution_set != 0x100 )
					contradiction = 1; 
				break;
			case 9: 
				if ( (game->grid[found]).solution_set != 0x200 )
					contradiction = 1; 
				break;
		}
	}

	(game->grid[found]).value = 0;	/* remove solved value...leave it for the user to guess */

	if (contradiction)
		return SUDOKU_CONTRADICTION;

	if (found != -1)
	{
		printf("Try %c%d\n", found % 9 + 65, found / 9 + 1);	/* print hint */
		return SUDOKU_SUCCESS;
	}

	printf("Sorry, I'm clueless.\n");
	return SUDOKU_NO_SOLUTION;
}

/*
  Sudoku_solve: find a solution for the Sodoku puzzle using the 
  algorithm in the specification document.
  Return the following error codes:
     SUDOKU_SUCCESS : solution found
     SUDOKU_NO_SOLUTION : no solution found using solver
     SUDOKU_CONTRADICTION : contradiction found
*/
int Sudoku_solve(void)
{
	/* derived from pseudocode taken from program specification */

	int found = 1;	/* found = true */
	int contradiction = 0;	/* contradiction = false */
	int solved = 1;	/* flag variable to determine if sudoku is solved */
	int i;	/* index variable */
	int j;	/* index variable */
	int k;	/* index variable */

	/* determine whether sudoku is already solved */
	for (i = 0; i < 81; i++)
	{
		if ( (game->grid[i]).value == 0 )
			solved = 0;
	}

	if (solved)
		return SUDOKU_SOLVED;
	
	/* while found is true */
	while (found)
	{
		found = 0;	/* found = false */

		/* inidialize solution set for each square */
		for (i = 0; i < 81; i++)
		{
			switch ( (game->grid[i]).value )
			{
				case 0:	(game->grid[i]).solution_set = 0x3FE; break;
				case 1: (game->grid[i]).solution_set = 0x2; break;
				case 2: (game->grid[i]).solution_set = 0x4; break;
				case 3: (game->grid[i]).solution_set = 0x8; break;
				case 4: (game->grid[i]).solution_set = 0x10; break;
				case 5: (game->grid[i]).solution_set = 0x20; break;
				case 6: (game->grid[i]).solution_set = 0x40; break;
				case 7: (game->grid[i]).solution_set = 0x80; break;
				case 8: (game->grid[i]).solution_set = 0x100; break;
				case 9: (game->grid[i]).solution_set = 0x200; break;
			}
		}

		for (i = 0; i < 81; i++)
		{
			/* reduce possible solutions based on rows */
			for (j = i - (i % 9); j < i - (i % 9) + 9; j++)
			{
				switch ( (game->grid[j]).value )
				{
					case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
					case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
					case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
					case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
					case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
					case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
					case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
					case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
					case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
				}	
			}

			/* reduce possible solutions based on columns */
			for (j = i % 9; j < 81; j+=9)
			{
				switch ( (game->grid[j]).value )
				{
					case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
					case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
					case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
					case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
					case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
					case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
					case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
					case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
					case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
				}
			}

			/* reduce possible solutions based on blocks */
			
			j = i - (i % 27) + ( (i % 9) / 3 ) * 3;
			k = j + 3;
			while (j < k)
			{
				switch ( (game->grid[j]).value )
				{
					case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
					case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
					case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
					case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
					case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
					case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
					case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
					case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
					case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
				}
				j++;				
			}

			j+=6;
			k = j + 3;
			while (j < k)
			{
				switch ( (game->grid[j]).value )
				{
					case 0:	break;
					case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
					case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
					case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
					case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
					case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
					case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
					case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
					case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
					case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
				}
				j++;				
			}

			j+=6;
			k = j + 3;
			while (j < k)
			{
				switch ( (game->grid[j]).value )
				{
					case 0:	break;
					case 1: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x2; break;
					case 2: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x4; break;
					case 3: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x8; break;
					case 4: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x10; break;
					case 5: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x20; break;
					case 6: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x40; break;
					case 7: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x80; break;
					case 8: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x100; break;
					case 9: (game->grid[i]).solution_set = (game->grid[i]).solution_set & ~0x200; break;
				}
				j++;				
			}	
		}

		for (i = 0; i < 81; i++)
		{
			/* for each square that is blank */
			if ( (game->grid[i]).value == 0 )
			{
				/* if there is only one valid solution, set the square to that solution */
				switch ( (game->grid[i]).solution_set )
				{
					case 0x2: (game->grid[i]).value = 1; break;
					case 0x4: (game->grid[i]).value = 2; break;
					case 0x8: (game->grid[i]).value = 3; break;
					case 0x10: (game->grid[i]).value = 4; break;
					case 0x20: (game->grid[i]).value = 5; break;
					case 0x40: (game->grid[i]).value = 6; break;
					case 0x80: (game->grid[i]).value = 7; break;
					case 0x100: (game->grid[i]).value = 8; break;
					case 0x200: (game->grid[i]).value = 9; break;
				}

				found = 1;	/* found = true */			
			}
		}

		for (i = 0; i < 81; i++)
		{
			/* for each square that is not blank */
			if ( (game->grid[i]).value )
			{
				/* if the solution set does not match the value, contradiction = true */
				switch ( (game->grid[i]).value )
				{
					case 1: 
						if ( (game->grid[i]).solution_set != 0x2 )
							contradiction = 1; 
						break;
					case 2: 
						if ( (game->grid[i]).solution_set != 0x4 )
							contradiction = 1; 
						break;
					case 3: 
						if ( (game->grid[i]).solution_set != 0x8 )
							contradiction = 1; 
						break;
					case 4: 
						if ( (game->grid[i]).solution_set != 0x10 )
							contradiction = 1; 
						break;
					case 5: 
						if ( (game->grid[i]).solution_set != 0x20 )
							contradiction = 1; 
						break;
					case 6: 
						if ( (game->grid[i]).solution_set != 0x40 )
							contradiction = 1; 
						break;
					case 7: 
						if ( (game->grid[i]).solution_set != 0x80 )
							contradiction = 1; 
						break;
					case 8: 
						if ( (game->grid[i]).solution_set != 0x100 )
							contradiction = 1; 
						break;
					case 9: 
						if ( (game->grid[i]).solution_set != 0x200 )
							contradiction = 1; 
						break;
				}
			}
		}
	}

	if (contradiction)
		return SUDOKU_CONTRADICTION;

	/* check to see if the sudoku was solved */
	solved = 1;
	for (i = 0; i < 81; i++)
	{
		if ( (game->grid[i]).value == 0 )
			solved = 0;
	}

	if (!solved)
		return SUDOKU_NO_SOLUTION;

	/* printf("Solve.\n"); */
	return SUDOKU_SUCCESS;
}

/*
  Sudoku_reset_game: reset the board to the beginning of play.
 */
void Sudoku_reset_game(void)
{
	int i;

	/*printf("Reset game.\n");*/

	for (i = 0; i < 81; i++)
	{
		(game->grid[i]).value = (game->backup[i]).value;
		(game->grid[i]).solution_set = (game->backup[i]).solution_set;
	}
}


/*
  Print the Sodoku game to the screen.  You must print a header
  row labeled from A to I.  You must print a leading digit for
  each row.
 */
void Sudoku_print(void)
{
	int i;
	int j;

	printf("    A B C   D E F   G H I\n");
	printf("   _______________________\n");
	for (i = 1; i <= 9; i++)
	{
		printf("%d | ", i);
		for (j = i * 9 - 9; j < i * 9; j++)
		{
			if (j % 3 == 2)
			{
				if ( (game->grid[j]).value != 0)
					printf("%d | ", (game->grid[j]).value );
				else
					printf("- | ");
			}	
			else
			{
				if ( (game->grid[j]).value != 0)
					printf("%d ", (game->grid[j]).value );
				else
					printf("- ");
			}
		}
		printf("\n");
		
		if ( !(i % 3) )
			printf("   -----------------------\n");
	}

	/*printf("Print game board.\n");*/
}




