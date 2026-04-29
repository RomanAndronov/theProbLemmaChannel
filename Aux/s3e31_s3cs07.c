#include <stdio.h>
#include <stdlib.h>

static int A[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
static size_t N = sizeof( A ) / sizeof( A[ 0 ] );

/*
  The input array, A, is assumed to
  be sorted in an ascending order:
  A[0] <= A[1] <= ... <= A[n-1]
 */
extern int
isSumOfTwoInts( int* A, size_t n,
	int x,
	size_t* i1, int* v1,
	size_t* i2, int* v2 )
{
	int		s;
	int*		lp = &A[ 0 ];
	int*		rp = &A[ n - 1 ];

	while ( lp != rp )
	{
		s = *lp + *rp;
		if ( s > x )
		{
			rp--;
		}
		else if ( s < x )
		{
			lp++;
		}
		else /* s == x */
		{
			*i1 = lp - A;
			*v1 = *lp;
			*i2 = rp - A;
			*v2 = *rp;
		return 1;
		}
	}

	return 0;
}

extern int
main( int argc, char* argv[] )
{
	size_t		ndx1;
	size_t		ndx2;
	int		val1;
	int		val2;
	int		x = atoi( argv[ 1 ] );
	int		rv = isSumOfTwoInts( A, N, x, &ndx1, &val1, &ndx2, &val2 );

	if ( rv )
	{
		printf( "%d = %d + %d = A[ %ld ] + A[ %ld ]\n",
			x, val1, val2, ndx1, ndx2 );
	}
	else
	{
		printf( "No two integers in A sum to %d\n", x );
	}

	return rv;
}
