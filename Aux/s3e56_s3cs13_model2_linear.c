/*
   This is an auxiliary code for Season 3 Episode 56
   Computer Science problem S3CS13.
   
   This file contains the code that implements
   Model 2, a linear interpretation of paper
   folding process
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

/*
  Solution: instead of actually folding the strip of
   paper keep it 'linear' but rearrange the numbers:

   for each fold:

    - reverse the numbers in the 'right' half of the
      strip by pushing them onto the stack in a
      right-to-left fashion: the right-most number is
      pushed first

    - insert the popped numbers at the proper location
      within the strip: the number of consecutive
      pops/inserts grows - as 2 raised to the power
      of (currFold - 1)

  argv[ 1 ] is the number of folds to perform

 */


static int		ListSz = 0;
static int		*List = NULL;

static int		StackSz = 0;
static int		*Stack = NULL;
static int		StackP = -1;


static int		init();
static void		fold();

static void		push( int );
static int		pop();
static void		printStack();

static void		insert( int, int, int );
static void		printList();
static void		printInOrder();


extern int
main( int argc, char* argv[] )
{
	int			nFolds;


	if ( argc < 2 )
	{
		return 1;
	}

	nFolds = atoi( argv[ 1 ] );
	if ( nFolds < 1 )
	{
		return 1;
	}

	if ( !init( nFolds ) )
	{
		printf( "%s: error: init() failed\n",
			argv[ 0 ] );
		return 1;
	}

	fold( nFolds );

	printList();

	free( List );
	free( Stack );

	return 0;
}


/*
  The variable 'ListSz' can always be
  initialized with a less readable:

   ListSz = 1 << nFolds;

 */
static int
init( int nFolds )
{
	int		i;
	int		e;


	ListSz = ( int )pow( 2, nFolds );

	List = calloc( ListSz, sizeof( int ) );
	e = errno;
	if ( !List )
	{
		printf( "calloc( list ) failed: errno %d: %s\n",
			e, strerror( e ) );
		return 0;
	}

	StackSz = ListSz / 2;
	Stack = calloc( StackSz, sizeof( int ) );
	e = errno;
	if ( !Stack )
	{
		free( List );
		List = NULL;
		ListSz = 0;
		printf( "calloc( stack ) failed: errno %d: %s\n",
			e, strerror( e ) );
		return 0;
	}

	for ( i = 1; i <= ListSz; i++ )
	{
		List[ i - 1 ] = i;
	}

	return 1;
}


static void
fold( int nFolds )
{
	int		i;
	int		currFold;
	int		nStackPasses;
	int		currStackPass;
	int		nConsecPops;
	int		currPop;
	int		nItemsAdded;
	int		at;
	int		h;
	int		half = ListSz / 2;


	/*
	  For each fold
	 */
	for ( currFold = 1; currFold <= nFolds; currFold++ )
	{
		/*
		  push all the numbers from the right half
		  of the strip onto the stack in a
		  right-to-left fashion: the right-most
		  item is pushed first

		  the length of the 'right half' of the
		  strip remains constant at all times - that,
		  partially, is what keeps the folded strip in
		  'linear' formation
		 */
		for ( h = half; h < ListSz; h++ )
		{
			push( List[ h ] );
		}


		/*
		  The variables below can always be set
		  with less readable:
		    nStackpasses = 1 << (nFolds - currFold);
		    nCoseqPops = 1 << (currFodl - 1);
		 */
		nStackPasses = ( int )pow( 2, nFolds - currFold );
		nConsecPops = ( int )pow( 2, currFold - 1 );

		/*
		  Pop the growing with each fold consecutive
		  number of items and insert them at the proper
		  location maintained in 'at'
		 */
		nItemsAdded = 0;
		currStackPass = 1;
		for ( ; currStackPass <= nStackPasses; currStackPass++ )
		{
			at = currStackPass * nConsecPops + nItemsAdded;
			currPop = 1;
			for ( ; currPop <= nConsecPops; currPop++ )
			{
				i = pop();
				insert( i, at, half + nItemsAdded );
				nItemsAdded++;
				at++;
			}
		}
	}
}


static void
push( int i )
{
	if ( StackP >= StackSz )
	{
		printf( "fold error: stack overflow\n" );
		exit( 1 );
	}
	StackP++;
	Stack[ StackP ] = i;
}


static int
pop()
{
	int		i;


	if ( StackP < 0 )
	{
		printf( "fold error: stack underflow\n" );
		exit( 1 );
	}

	i = Stack[ StackP ];
	StackP--;

	return i;
}


static void
printStack()
{
	int		i;


	printf( "Stack:\n" );
	if ( StackP < 0 )
	{
		printf( "\tis empty\n" );
		return;
	}

	for ( i = StackP; i >= 0; i-- )
	{
		printf( "\t%d\n", Stack[ i ] );
	}
}

static void
insert( int i, int at, int sz )
{
	int		j;
	int		nitmv = sz - at;


	if ( nitmv > 0 )
	{
		memmove( &List[ at + 1 ], &List[ at ], nitmv * sizeof( int ) );
	}


	List[ at ] = i;
}


static void
printList()
{
	int		i;


	for ( i = 1; i <= ListSz; i++ )
	{
		printf( "%3d ", i );
	}
	printf( "\n" );

	for ( i = 0; i < ListSz; i++ )
	{
		printf( "%3d ", List[ i ] );
	}
	printf( "\n" );
}


static int
find( int n )
{
	int		ndx;

	for ( ndx = 0; ndx < ListSz; ndx++ )
	{
		if ( List[ ndx ] == n )
		{
			return ndx;
		}
	}

	return -1;
}

static void
printInOrder()
{
	int		i;
	int		ndx;


	for ( i = 0; i < ListSz; i++ )
	{
		ndx = find( i + 1 );
		printf( "%d-%d ", i + 1, ndx + 1 );
	}
	printf( "\n" );
}
