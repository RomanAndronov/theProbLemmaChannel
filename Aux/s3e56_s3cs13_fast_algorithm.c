/*
   This is an auxiliary code for Season 3 Episode 56
   Computer Science problem S3CS13.
   
   This file contains the code that implements
   a fast algorithm for finding the position
   of an arbitrary input number in the output
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/*
  Find the new position of a given number within the
  strip that has been folded a given number of times

  argv[ 1 ] is the number whose new position is
   sought-after

  argv[ 2 ] is the number of times the strip is
   to be folded

 */


#define INT_BIT_SZ		( sizeof( int ) * 8 )

static unsigned int		mkMask( unsigned int );
static unsigned int		reverseBits( unsigned int, unsigned int );


extern int
main( int argc, char* argv[] )
{
	unsigned long		ul;
	unsigned int		x;
	unsigned int		nFolds;
	unsigned int		N;
	unsigned int		half;
	unsigned int		xprime;
	unsigned int		mask;


	/*
	  Validation
	 */
	if ( argc != 3 )
	{
		return 0;
	}

	errno = 0;
	ul = strtoul( argv[ 1 ], NULL, 10 );
	if ( errno || ul < 1 || ul > INT_MAX )
	{
		return 0;
	}
	x = ul;

	errno = 0;
	ul = strtoul( argv[ 2 ], NULL, 10 );
	if ( errno )
	{
		return 0;
	}
	nFolds = ul;
	if ( nFolds < 1 || nFolds > INT_BIT_SZ )
	{
		return 0;
	}

	N = 1 << nFolds;
	if ( x > N )
	{
		return 0;
	}


	/*
	  Algorithm:

	  subtract 1
	 */
	x = x - 1;

	/*
	  Divide by 2
	 */
	half = x >> 1;

	/*
	  Reverse the bits in 'half'
	 */
	xprime = reverseBits( half, nFolds );

	/*
	  Flip the bits if 'x' is odd
	*/
	if ( x % 2 )
	{
		xprime = ~xprime;
		mask = mkMask( nFolds );
		xprime = xprime & mask;
	}


	printf( "x'(%u, %u) = %u\n",
		x + 1, nFolds, xprime + 1 );

	return 0;
}


/*
  The mask is used to zero out ( INT_BIT_SZ - nFolds )
  bits in the variable 'half' if 'x' is odd, see main()

  The mask itself is 'nFolds' consecutive 1s

 */
static unsigned int
mkMask( unsigned int nFolds )
{
	unsigned int		i;
	unsigned int		mask = 0;

	for ( i = 0; i < nFolds; i++ )
	{
		mask = mask | ( 1 << i );
	}

	return mask;
}


/*
  Bit reversal is done with respect to 'nFolds' - as
  if the binary object is 'nFolds' bits long:

  - reverse the bits in unsigned int

  - shift them right ( INT_BIT_SZ - nFolds ) times

 */
static unsigned int
reverseBits( unsigned int n, unsigned int l )
{
	int			i;
	unsigned int		tmp;
	unsigned int		rn = 0;

	for ( i = 0; i < INT_BIT_SZ; i++ )
	{
		tmp = n & ( 1 << i );
		if ( tmp )
		{
			rn |= 1 << ( INT_BIT_SZ - 1 - i );
		}
	}

	rn = rn >> ( INT_BIT_SZ - l );

	return rn;
}
