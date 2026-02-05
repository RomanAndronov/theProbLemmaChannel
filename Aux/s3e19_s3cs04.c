/*
  This file contains the C code that implements
  the idea discussed in Season 3 Episode 19, in
  which we solve the Computer Science problem
  S3CS4.

  The code is provided "as is" without warranties
  of any code. Use this code at your own risk.

  Compile this file as follows:

   gcc -g -o s3e19_s3cs04 s3e19_s3cs04.c

  Test it as follows:

   ./s3e19_s3cs04 7 5 "1234567ABCDE"

  and observe the result. Feel free to modify
  this code as you see fit.
 */

#include <stdio.h>
#include <stdlib.h>

void
printArray( char* a, size_t asz )
{
	size_t			i;

	for ( i = 0; i < asz; i++ )
	{
		printf( "'%c' ", a[ i ] );
	}

	printf( "\n" );
}

size_t
modOp( size_t n, size_t m, char op )
{
	if ( op == '+' )
	{
		return ( n + 1 ) % m;
	}

	if ( n == 0 )
	{
		return m - 1;
	}

	return n - 1;
}

size_t
reflectArray( char* a, size_t aSz,
	size_t gNdx, size_t dNdx,
	size_t N )
{
	char			tmp;
	size_t			i;
	size_t			g = gNdx;
	size_t			d = dNdx;

	for ( i = 0; i < N; i++ )
	{
		tmp = a[ g ];
		a[ g ] = a[ d ];
		a[ d ] = tmp;
		g = modOp( g, aSz, '+' );
		d = modOp( d, aSz, '-' );
	}

	return 0;
}

extern int
main( int argc, char* argv[] )
{
	char* 			p;
	char			a[ 256 ];
	size_t			N;
	size_t			gNdx;
	size_t			dNdx;
	size_t			i = 0;
	size_t			l = atol( argv[ 1 ] );
	size_t			r = atol( argv[ 2 ] );
	size_t			asz = l + r;

	/*
	  Fill in our toy array with
	  the data supplied by the user
	*/
	p = ( char* )argv[ 3 ];
	while ( *p )
	{
		a[ i++ ] = *p++;
	}

	/*
	  Show the original
	*/
	printArray( a, asz );

	N = asz / 2;

	/*
	  The first axis of reflection
	  always passes between the first
	  and the last element
	 */
	reflectArray( a, asz, 0, asz - 1, N );

	/*
	  Show the intermediate result
	 */
	printArray( a, asz );

	/*
	  Locate the position of the
	  second reflection axis. Adjust
	  the number of reflections as needed
	 */
	if ( r % 2 )
	{
		gNdx = modOp( r / 2, asz, '+' );
		dNdx = modOp( r / 2, asz, '-' );
		if ( l % 2 )
		{
			N = N - 1;
		}
	}
	else
	{
		gNdx = r / 2;
		dNdx = modOp( r / 2, asz, '-' );
	}

	reflectArray( a, asz, gNdx, dNdx, N );

	printArray( a, asz );

	return 0;
}
