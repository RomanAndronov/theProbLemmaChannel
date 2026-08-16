/*
   This is an auxiliary code for Season 3 Episode 56
   Computer Science problem S3CS13.
   
   This file contains the code that implements
   Model 1, a literal interpretation of paper
   folding process
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

/*
  Solution: literal.

   Every number has two immediate neighbors - above and below. Initially,
   numbers have no neighbors. The actual folding of a strip is split
   across two stages:

   - moving horizontally to pair up each remaining strip's 'left' column
     (located in the left half) with the corresponding 'right' column
     (located in the right half) that will soon be placed on top of the
     'left' column

   - moving vertically through the folded layers to connect the stack of
     numbers in the 'left' column to the stack of numbers in the
     corresponding 'right' column where 'connecting' means reversing the
     order of numbers in the 'right' column since they are being turned
     up side down as a result of folding/rotating

  argv[ 1 ] is the number of folds to perform

 */


#define NIL	-1

/*
  A number in a strip
 */
typedef struct
{
	int	numAbove;
	int	numBelow;
} num_t;


static num_t*		init( int );
static void		fold( num_t*, int, int );


extern int
main( int argc, char* argv[] )
{
	int		N;
	int		i;
	int		half;
	int		numAbove;
	int		leftCol;
	int		rightCol;
	int		nFolds;
	int		currFold;
	num_t		*strip;


	if ( argc != 2 )
	{
		return 0;
	}

	nFolds = atoi( argv[ 1 ] );
	if ( nFolds < 1 )
	{
		return 0;
	}

	strip = init( nFolds );
	if ( !strip )
	{
		printf( "%s: error: init() failed\n",
			argv[ 0 ] );
		return 0;
	}


	/*
	  The variable 'N' can always be
	  initialized with less readable:
	   N = 1 << nFolds
	 */
	N = ( int )pow( 2, nFolds );


	/*
	  In this loop we move horizontally, along the OX axis,
	  left to right, from the strip's fixed left-most square
	  toward its current middle square, which will become the
	  strip's right-most square after the current fold is
	  complete - that right-most square will then be rotated
	  about the new middle toward the fixed left-most square
	  of the strip and so on
	 */
	for ( currFold = 1; currFold <= nFolds; currFold++ )
	{
		/*
		  Mark two halves of the strip: 'left' and 'right'
		 */
		half = N / 2;

		/*
		  Pair up each 'left' column with its corresponding
		  'right' image with respect to a vertical passing
		  through the strip's current middle
		 */
		for ( leftCol = 1; leftCol <= half; leftCol++ )
		{
			/*
			   The corresponding 'right' column that
			   is to be placed on top of the 'left'
			   column
			 */
			rightCol = N - leftCol + 1;

			/*
 			  Here we move vertically, along the OZ axis,
			  upward, through strip's multiple layers and
			  connect the stack of numbers in the 'left'
			  column to the stack of numbers in the 'right'
			  column reversing the order of items in the
			  latter
			 */
			fold( strip, leftCol, rightCol );
		}

		/*
		  Complete the current fold
		 */
		N /= 2;
	}

	/*
	  Numbers in original order
	 */
	N = ( int )pow( 2, nFolds );
	for ( i = 1; i <= N; i++ )
	{
		printf( "%3d ", i );
	}
	printf( "\n" );
	 
	/*
	  Numbers in new order
	 */
	numAbove = 1;
	while ( numAbove != NIL )
	{
		printf( "%3d ", numAbove );
		numAbove = strip[ numAbove ].numAbove;
	}
	printf( "\n" );

	free( strip );

	return 0;
}


/*
  The vairable 'n' below can always be
  initialized with less readable:

   n = 1 << nFolds + 1

  where 'plus one' is used because we
  access the numbers in the array in
  their natural, 1-based, and not C,
  0-based, order

 */
static num_t*
init( int nFolds )
{
	int		i;
	int		e;
	int		n = ( int )pow( 2, nFolds ) + 1;
	num_t		*strip = calloc( n, sizeof( num_t ) );

	e = errno;
	if ( !strip )
	{
		printf( "calloc() failed: errno %d: %s\n",
			e, strerror( e ) );
		return strip;
	}

	for ( i = 1; i < n; i++ )
	{
		strip[ i ].numAbove = NIL;
		strip[ i ].numBelow = NIL;
	}

	return strip;
}


/*
  Fold the given 'rightColumn' of a strip up and
  over and place it on top of the corresponding
  'leftColumn'

  'Left' and 'right' columns are symmetrical with
  respect to a vertical passing through the strip's
  current middle

  For example:

   if the strip's current length is 8 then
    to left column 1 corresponds right column 8
    to left column 2 corresponds right column 7
    to left column 3 corresponds right column 6
    to left column 4 corresponds right column 5
     etc

 */
static void
fold( num_t *strip, int leftColumn, int rightColumn )
{
	int		numAtTopLeftHalf;
	int		numAtTopRightHalf;
	int		oldNumBelow;


	/*
	  Go up to the upper-most layer of the
	  strip's left column
	 */
	numAtTopLeftHalf = leftColumn;
	while ( strip[ numAtTopLeftHalf ].numAbove != NIL )
	{
		numAtTopLeftHalf = strip[ numAtTopLeftHalf ].numAbove;
	}


	/*
	  Go up to the upper-most layer of the
	  strip's right column - the one being
	  folded/rotated up and over to be placed
	  on top of the strip's given left column
	 */
	numAtTopRightHalf = rightColumn;
	while ( strip[ numAtTopRightHalf ].numAbove != NIL )
	{
		numAtTopRightHalf = strip[ numAtTopRightHalf ].numAbove;
	}


	/*
	  Perform the actual folding by rotating the
	  right column up and over and placing it on
	  top of the left column:

	  1) connect the number at the top of the left
	   column to a new (formerly right) column of
	   numbers - starting from a number at the top
	   of the (formerly) right column

	  2) reverse the 'above' and 'below' links
	   in the (formerly) right column

	  For example:

	   if an 8-unit long strip was folded once and
	   this is the second fold and the right column
	   3 is folded up and over to be placed on top
	   of the left column 2 then:

	   1) the number at the top of the left column,
	    7, must be connected to the number at the top
	    of the (formerly) right column, 6:
	     7.numAbove = 6

	   2) the original order of numbers in the
	   (formerly) right column, 6 on top of 3, must
	   be reversed to: 3 on top of 6

	 */
	while ( numAtTopRightHalf != NIL )
	{
		/*
		  A new neighbor above
		 */
		strip[ numAtTopLeftHalf ].numAbove = numAtTopRightHalf;

		/*
 		  Remember the old neighbor below
		 */
		oldNumBelow = strip[ numAtTopRightHalf ].numBelow;

		/*
 		  Get a new neighbor below
		 */
		strip[ numAtTopRightHalf ].numBelow = numAtTopLeftHalf;

		/*
 		  Switch
		 */
		numAtTopLeftHalf = numAtTopRightHalf;

		/*
		  Move on to the next number in the
		  (formerly) right column
		 */
		numAtTopRightHalf = oldNumBelow;
	}

	/*
	  This is a new upper-most number in the
	  left column - there is nothing above it
	 */
	strip[ numAtTopLeftHalf ].numAbove = NIL;
}
