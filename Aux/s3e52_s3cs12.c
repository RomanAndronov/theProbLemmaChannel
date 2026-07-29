/*
  This is a supplementary code for the
  ProbLemma's Channel S3E52/S3CS12.

  Use this, as is, code at your own risk
 */

#include <stdio.h>

typedef struct NODE
{
	char		data;
	struct NODE*	next;
} node_t;

extern node_t*
reverseList( node_t* startHere )
{
	node_t*			prev = NULL;
	node_t*			curr = startHere;
	node_t*			next = NULL;

	while ( curr )
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	return prev;
}

extern void
printList( node_t* startHere )
{
	node_t*			curr = startHere;

	while ( curr )
	{
		printf( "%c -> ", curr->data );
		curr = curr->next;
	}
	printf( "null\n" );
}

extern int
main( int argc, char* argv[] )
{
	node_t		list[] =
{
	{'A', NULL}, {'B', NULL},
	{'C', NULL}, {'D', NULL},
	{'E', NULL}, {'F', NULL},
	{'G', NULL}
};

	list[ 0 ].next = &list[ 1 ];
	list[ 1 ].next = &list[ 2 ];
	list[ 2 ].next = &list[ 3 ];
	list[ 3 ].next = &list[ 4 ];
	list[ 4 ].next = &list[ 5 ];
	list[ 5 ].next = &list[ 6 ];

	printList( &list[ 0 ] );

	reverseList( &list[ 0 ] );
	printList( &list[ 6 ] );

	reverseList( &list[ 6 ] );
	printList( &list[ 0 ] );

	return 0;
}
