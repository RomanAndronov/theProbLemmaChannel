/*
  This is an auxiliary code for S3E72/S3CS17.
  It implements an algorithm for printing
  all prime numbers between 2 and 100 without
  using number literals or ASCII characters
  using a naive version of the Sieve of
  Eratosthenes
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern int
main( int argc, char* argv[] )
{
	long			n;
	long*			primeSuspects;

	uintptr_t		mainAddr = ( uintptr_t )main;
	long			one = ( long )( mainAddr / mainAddr );
	long			zero = one - one;
	long			two = one + one;
	long			five = two + two + one;
	long			oneHundred = two * two * five * five;

	/*
	  Allocate a list of prime suspects on the heap
	 */
	n = oneHundred;
	primeSuspects = ( long* )malloc( ( n + one ) * sizeof( long ) );

	/*
	  Initially everyone is a prime suspect
	 */
	for( long i = two; i <= n; i++ )
	{
		primeSuspects[ i ] = one;
	}

	/*
	  Roll through the sieve by probing only those
	  primes that are <= sqrt(n) but eliminate the
	  prime suspects from the entire list
	 */
	for ( long p = two; p*p <= n; p++ )
	{
		if ( primeSuspects[ p ] == one )
		{
			printf( "Eliminating %ld-prime suspects\n", p );
			for ( long m = p*p; m <= n; m += p )
			{
				primeSuspects[ m ] = zero; /* Eliminated! */
			}
		}
	}

	/*
	  Lastly, roll through the sieve one more time
	  and print out the genuine prime numbers only
	 */
	for ( long p = two; p <= n; p++ )
	{
		if ( primeSuspects[ p ] == one )
		{
			printf( "%ld ", p );
		}
	}

	printf( "\n" );
	free( primeSuspects );

	return ( int )zero;
}
