#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
 
static char* Alphabet = "0123456789" 
 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
 
static void 
mkBottleSignature( char* s, int bnm, int base, int X ) 
{ 
	int			i; 
	size_t			ndx; 
	 
	for ( i = 0; i < X; i++ ) 
	{ 
		s[ i ] = '0'; 
	} 
	 
	while ( X ) 
	{ 
		ndx = bnm % base; 
		s[ X - 1 ] = Alphabet[ ndx ]; 
		bnm = bnm / base; 
		X--; 
	} 
} 
 
extern int 
main( int argc, char* argv[] ) 
{ 
	int			k; 
	int			j; 
	int			s; 
	int			first; 
 
	char			sig[ 16 ] = { 0 }; 
	int			n = atoi( argv[ 1 ] ); 
	int			nt = atoi( argv[ 2 ] ); 
	int			b = nt + 1; 
	int			X = ceil( log( n ) / log( b ) ); 
 
	printf( "Test %d bottles over %d tastings with %d mice.\n", 
		n, nt, X ); 
 
	printf( "Bottles' base-%d signatures:\n", b ); 
	for ( s = 0; s < n; s++ ) 
	{ 
		mkBottleSignature( sig, s, b, X ); 
		printf( " %d = %s\n", s, sig ); 
	} 
 
	for ( k = 0; k < nt; k++ ) 
	{ 
		printf( "Test number %d:\n", k ); 
		for ( j = 0; j < X; j++ ) 
		{ 
			printf( " mouse number %d: {", j ); 
			first = 1; 
			for ( s = 0; s < n; s++ ) 
			{ 
				mkBottleSignature( sig, s, b, X ); 
				if ( sig[ X - 1 - j ] == Alphabet[ k ] ) 
				{ 
					if ( first != 1 ) 
					{ 
					printf( ", " ); 
					} 
					if ( first == 1 ) 
					{ 
					first = 0; 
					} 
					printf( "%d", s ); 
				} 
			} 
			printf( "}\n" ); 
		} 
	} 
 
	return 0; 
} 
