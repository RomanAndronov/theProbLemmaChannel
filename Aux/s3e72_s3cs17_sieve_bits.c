/*
  This is an auxiliary code for S3E72/S3CS17.
  It implements an algorithm for printing
  all prime numbers between 2 and 100 without
  using number literals or ASCII characters
  using a space-optimized version of the Sieve
  of Eratosthenes by allocating not an array
  of n long integers but an array of bytes
  that can hold n BITS
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
	long		byteIndex;
	long		bitIndex;
} bit_location_t;

extern bit_location_t*
getBitLocation( long bitNumber,
	long three,
	long seven,
	bit_location_t* bl )
{
	/*
	  This is a fast version of division by
	  8, since 8 is the 3-rd power of 2
	 */
	bl->byteIndex = bitNumber >> three;

	/*
 	  And this is a fast version of computing
	  residue modulo 8 because such a residue
	  can only by a number [0-7] and it takes
	  only 3 bits to capture such a number.
	  Hence, in base two "seven" is exactly
	  a 3-bit mask 00000111 that kills the
	  upper 5 bits and only copies the lower
	  3 bits which is the residue modulo 8
	 */
	bl->bitIndex = bitNumber & seven;

	return bl;
}

extern long
getBit( uint8_t* bits,
	long bitNumber,
	long one,
	long three,
	long seven )
{
	bit_location_t		bl;

	getBitLocation( bitNumber, three, seven, &bl );

	return ( bits[ bl.byteIndex ] >> bl.bitIndex ) & one;
}

extern void
setBit( uint8_t* bits,
	long bitNumber,
	long value,
	long one,
	long three,
	long seven )
{
	bit_location_t		bl;

	getBitLocation( bitNumber, three, seven, &bl );

	if ( value )
	{
		bits[ bl.byteIndex ] |= ( uint8_t )( one << bl.bitIndex );
	}
	else
	{
		bits[ bl.byteIndex ] &= ( uint8_t )~( one << bl.bitIndex );
	}
}

extern int
main( int argc, char* argv[] )
{
	long			n;
	long			byteSize;
	uint8_t*		primeSuspects;

	uintptr_t		mainAddr = ( uintptr_t )main;
	long			one = ( long )( mainAddr / mainAddr );
	long			zero = one - one;
	long			two = one + one;
	long			three = two + one;
	long			five = two + two + one;
	long			seven = five + two;
	long			oneHundred = two * two * five * five;

	/*
	  Allocate a list of prime suspects on the heap
	 */
	n = oneHundred;
	byteSize = ( n + seven ) >> three;
	primeSuspects = ( uint8_t* )malloc( byteSize * sizeof( uint8_t ) );

	/*
	  Initially everyone is a prime suspect.
	 */
	for( long i = zero; i < byteSize; i++ )
	{
		primeSuspects[ i ] = ( uint8_t )~zero;
	}

	/*
	  Roll through the sieve by probing only those
	  primes that are <= sqrt(n) but eliminate the
	  prime suspects from the entire list
	 */
	for ( long p = two; p*p <= n; p++ )
	{
		if ( getBit( primeSuspects, p, one, three, seven ) )
		{
			printf( "Eliminating %ld-prime suspects\n", p );
			for ( long m = p*p; m <= n; m += p )
			{
				setBit( primeSuspects, m, zero, one, three, seven ); /* Eliminated! */
			}
		}
	}

	/*
	  Lastly, roll through the sieve one more time
	  and print out the genuine prime numbers only
	 */
	for ( long p = two; p <= n; p++ )
	{
		if ( getBit( primeSuspects, p, one, three, seven ) )
		{
			printf( "%ld ", p );
		}
	}

	printf( "\n" );
	free( primeSuspects );

	return ( int )zero;
}
