/*
  This is an auxiliary code for S3E72/S3CS17.
  It implements an algorithm for printing
  all prime numbers between 2 and 100 without
  using number literals or ASCII characters
  using a classical definition of a prime number
 */

#include <stdio.h>
#include <stdint.h>

extern int
main( int argc, char* argv[] )
{
	long			n;
	long			d;
	long			isPrime;

	uintptr_t		mainAddr = ( uintptr_t )main;
	long			one = ( long )( mainAddr / mainAddr );
	long			zero = one - one;
	long			two = one + one;
	long			five = two + two + one;
	long			oneHundred = two * two * five * five;

	for ( n = two; n <= oneHundred; n = n + one )
	{
		isPrime = one;
		for ( d = two; d < n; d = d + one )
		{
			if ( ( n % d ) == zero )
			{
				isPrime = zero;
				break;
			}
		}
		if ( isPrime == one )
		{
			printf( "%ld ", n );
		}
	}

	printf( "\n" );
	return ( int )zero;
}
