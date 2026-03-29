/*
  This a Recursive Permutations Generator (rpg)
  instrumented to visually portray the underlying
  recursive calls
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N = 0;

void
Indent( int k )
{
	int			i;

	for ( i = 0; i < k; i++ )
	{
		putchar( '\t' );
	}
}

void
PrintB( int *b, int n, int k )
{
	int			i;

	Indent( k );
	printf( "B-array: \n" );
	Indent( k );
	for ( i = 0; i < n; i++ )
	{
		printf( "%3d", b[ i ] );
	}
	printf( "\n" );
}

void
PrintPerm( int *p, int n, int k )
{
	int			i;

	Indent( k );
	printf( "Current permutation: \n" );
	Indent( k );
	for ( i = 0; i < n; i++ )
	{
		printf( "%3d", p[ i ] );
	}
	printf( "\n" );
}

void
GenPerm( int *p, int *b, int i, int n )
{
	++N;
	Indent( i );
	printf( "Enter GenPerm(): i = %d\n", i );
	if ( i == n )
	{
		PrintPerm( p, n, i );
	}
	else
	{
		int		j;

		PrintB( b, n, i );
		for ( j = 0; j < n; j++ )
		{
			if ( b[ j ] == 0 )
			{
				b[ j ] = 1;
				p[ i ] = j + 1;
				Indent( i );
				printf( "Begin recursive call to GenPerm() with i = %d, j = %d\n", i + 1, j );
				GenPerm( p, b, i + 1, n );
				Indent( i );
				printf( "End recursive call to GenPerm(): i = %d, j = %d\n", i, j );
				b[ j ] = 0;
			}
			else
			{
				Indent( i );
				printf( "Skipping b[ %d ] not equal to 0\n", j );
			}
		}
		PrintB( b, n, i );
	}
	Indent( i );
	printf( "Exit GenPerm(): i = %d\n", i );
}

extern int
main( int argc, char* argv[] )
{
	int			*p;
	int			*b;
	int			n = atoi( argv[ 1 ] );

	p = ( int* )malloc( n * sizeof( int ) );
	b = ( int* )calloc( n, sizeof( int ) );

	GenPerm( p, b, 0, n );

	free( p );
	free( b );

	printf( "GenPerm() was called %d times\n", N );

	return 0;
}
