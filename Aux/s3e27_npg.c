/*
  This is a Narayana's Permutations Generator (npg)
 */

#include <stdio.h>
#include <stdlib.h>

static void
PrintMe( int* a, size_t asz )
{
	size_t			i;

	for ( i = 0; i < asz; i++ )
	{
		printf( "%d", a[ i ] );
		if ( i < asz - 1 )
		{
			printf( ", " );
		}
		else
		{
			printf( "\n" );
		}
	}
}

static
void Swap( int* a, int* b )
{
	int			tmp = *a;

	*a = *b;
	*b = tmp;
}

static void
reverseTilEnd( int* a, size_t ndx, size_t asz )
{
	size_t			i;

	a = a + ndx;
	asz = asz - ndx;
	for ( i = 0; i < asz / 2; i++ )
	{
		Swap( &a[ i ], &a[ asz - 1 - i ] );
	}
}

static int
findRightNeighbor( int* P, size_t PSz )
{
	int   k;

	for ( k = PSz - 2; k >= 0; k-- )
	{
		if ( P[ k ] < P[ k + 1 ] )
		{
			return k;
		}
	}

	return -1;
}

static int
findLargerElementThan( int* P, size_t PSz, int thisGuy )
{
	int   j;

	for ( j = PSz - 1; j >= 0; j-- )
	{
		if ( thisGuy < P[ j ] )
		{
			return j;
		}
	}

	return -1;
}

static void
orchestrate( int* P, size_t PSz, void ( *callMe )( int*, size_t ) )
{
	size_t			k;
	size_t			j;

	for ( k = 0; k < PSz; k++ )
	{
		P[ k ] = k + 1;
	}

	callMe( P, PSz );
	k = findRightNeighbor( P, PSz );
	while ( k != -1 )
	{
		j = findLargerElementThan( P, PSz, P[ k ] );
		Swap( &P[ k ], &P[ j ] );
		reverseTilEnd( P, k + 1, PSz );
		callMe( P, PSz );
		k = findRightNeighbor( P, PSz );
	}
}

extern int
main( int argc, char* argv[] )
{
	int*			P;
	size_t			PSz = atoi( argv[ 1 ] );

	P = ( int* )calloc( PSz, sizeof( int ) );
	orchestrate( P, PSz, PrintMe );
	free( P );

	return 0;
}
