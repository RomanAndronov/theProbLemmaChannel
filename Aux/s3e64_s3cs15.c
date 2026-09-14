/*
  This is an auxiliary code for S3E64.
  It implements an algorithm for printing
  all subsets of a given finite set to stdout
 */

#include <stdio.h>
#include <stdlib.h>

static void
printSubset( char* word, unsigned int n )
{
	int			i;
	int			first = 1;

	for ( i = 0; i < n; i++ )
	{
		if ( word[ i ] )
		{
			if ( first )
			{
				first = 0;
			}
			else
			{
				printf( " " );
			}
				printf( "%d", i + 1 );
		}
	}
	printf( "\n" );
}

static int
incrementByOne( char* word, unsigned int n )
{
	int			i;

	for ( i = 0; ( i < n ) && word[ i ] != 0; i++ )
	{
		word[ i ] = 0;
	}

	if ( i < n )
	{
		word[ i ] = 1;
		return 1;
	}

	return 0;
}

extern int
main( int argc, char* argv[] )
{
	int			n = atoi( argv[ 1 ] );
	char*			word = ( char* )calloc( n, n * sizeof( char ) );

	do
	{
		printSubset( word, n );
	} while ( incrementByOne( word, n ) );

	free( word );

	return 0;
}
