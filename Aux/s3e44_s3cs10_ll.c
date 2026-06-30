/*
  This is an auxiliary code for S3E44.
  It implements a singly-linked list
  whose memory is backed by an array
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct NODE
{
	struct NODE*		nextFree;
	char			data; /* some payload */
} node_t;

node_t*		gStore = NULL;
node_t*		gFirstFreeNode = NULL;

extern size_t
storeInit( node_t* store, size_t n )
{
	gFirstFreeNode = &gStore[ 0 ];

	for ( size_t i = 0; i < (n - 1); i++ )
	{
		store[ i ].data = 'a' + i;
		store[ i ].nextFree = &store[ i + 1 ];
	}

	store[ n - 1 ].data = 'a' + n - 1;
	store[ n - 1 ].nextFree = NULL;

	return n;
}

extern node_t*
storeAlloc()
{
	node_t*			freeNode = NULL;

	if ( gFirstFreeNode )
	{
		freeNode = gFirstFreeNode;
		gFirstFreeNode = freeNode->nextFree;
	}

	return freeNode;
}

extern void
storeFree( node_t* node )
{
	node->nextFree = gFirstFreeNode;
	gFirstFreeNode = node;
}

extern void
storePrint()
{
	node_t*			node = gFirstFreeNode;

	while ( node )
	{
		printf( "Node at %p: data = %c\n",
			node, node->data );
		node = node->nextFree;
	}
}

extern int
main( int argc, char* argv[] )
{
	node_t*			node;
	size_t			storeSize = 7;

	gStore = calloc( sizeof( node_t ), storeSize );

	storeInit( gStore, storeSize );
	printf( "A new store:\n" );
	storePrint();

	node = storeAlloc();
	printf( "Allocated node: %p: data = %c\n",
		node, node->data );
	storePrint();

	printf( "A store restored:\n" );
	storeFree( node );
	storePrint();

	free( gStore );

	return 0;
}
