/*
  This is an auxiliary code for S3E48.
  It implements a thread-safe fixed-size
  blocking queue based on the ideas covered
  in S3E44
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/*
  This structure stores the actual data
  that goes into a MT-safe queue. Add
  more fodder here as needed
 */
typedef struct mtq_entry
{
	struct mtq_entry*	next;
	char			data;
} mtq_entry_t;


/*
  This a structure that wraps everything that
  we call "a queue" under one umbrella
 */
typedef struct mtq
{
	mtq_entry_t*		memStore;
	size_t			memStoreSize;
	mtq_entry_t*		headFreeList;

	mtq_entry_t*		qHead;
	mtq_entry_t*		qTail;

	size_t			qCurrSize;

	pthread_mutex_t		lock;
	pthread_cond_t		cv;
} mtq_t;


/*
  Private interface:
   - for this interface it is assumed that the
     queue lock is held by the public functions,
     see below. That is why the names of the
     private functions are terminated with the
     "_nl" suffix, which means "No Lock" (is held)
 */
static mtq_entry_t*
storeAlloc_nl( mtq_t* q )
{
	mtq_entry_t*			freeEntry = NULL;

	if ( q->headFreeList )
	{
		freeEntry = q->headFreeList;
		q->headFreeList = freeEntry->next;
	}

	return freeEntry;
}

static void
storeFree_nl( mtq_t* q, mtq_entry_t *qe )
{
	qe->next = q->headFreeList;
	q->headFreeList = qe;
}


/*
  This will initialize a queue by creating
  its free memory store on the heap. In
  that state the queue is empty
 */
extern mtq_t*
mtqInit( mtq_t *mtq, size_t qsize )
{
	mtq->memStoreSize = qsize;
	mtq->memStore = ( mtq_entry_t* )calloc( mtq->memStoreSize, sizeof( mtq_entry_t ) );
	if ( !mtq->memStore )
	{
		return NULL;
	}

	/*
	  Initially the entire memory store is available
	 */
	mtq->headFreeList = &mtq->memStore[ 0 ];
	for ( size_t i = 0; i < ( mtq->memStoreSize - 1 ); i++ )
	{
		mtq->memStore[ i ].next = &mtq->memStore[ ( i + 1 ) % mtq->memStoreSize ];
	}

	/*
	  The last entry in the memory store points to null.
	  Thus, if "headFreeStore" pointer is null then this
	  queue is full
	 */
	mtq->memStore[ mtq->memStoreSize - 1 ].next = NULL;

	/*
	  The queue itself is empty
	 */
	mtq->qCurrSize = 0;
	mtq->qHead = mtq->qTail = NULL;

	pthread_mutex_init( &mtq->lock, NULL );
	pthread_cond_init( &mtq->cv, NULL );

	return mtq;
}


/*
  Adding a new entry to a queue means:
   - grab a free entry from the memory store
   - add that new entry to the tail (end) of the queue

  If this queue is currently full then this function
  will block until there is room in the queue for at
  least one entry.

  This function always returns the current size or
  the current number of entries in the queue, including
  the just-added entry
 */
extern size_t
mtqAdd( mtq_t* q, char data )
{
	size_t				qCurrSz = 0;
	mtq_entry_t*			newQEntry = NULL;

	pthread_mutex_lock( &q->lock );

		/*
		  Wait for free memory
		 */
		while ( q->headFreeList == NULL )
		{
			pthread_cond_wait( &q->cv, &q->lock );
		}

		/*
		  At this point we are good, the
		  "newQEntry" pointer is not null
		 */
		newQEntry = storeAlloc_nl( q );

		/*
		  Clean that entry up
		 */
		newQEntry->next = NULL;

		/*
		  Copy the data in
		 */
		newQEntry->data = data;

		/*
		  Add this entry to the tail of the queue
		 */
		if ( q->qHead == NULL )
		{
			q->qHead = q->qTail = newQEntry;
		}
		else
		{
			q->qTail->next = newQEntry;
			q->qTail = newQEntry;
		}

		qCurrSz = ++( q->qCurrSize );

		/*
		  Let the threads who are blocked in the
		  mtqRm() call know that there is material
		  in the queue that is available for removal
		 */
		pthread_cond_signal( &q->cv );

	pthread_mutex_unlock( &q->lock );

	return qCurrSz;
}


/*
  Removing a entry from a queue means:
   - remove an entry from the head (front) of the queue
   - return that entry to the memory store

  If this queue is currently empty then this function
  will block until there is at least one entry in that
  queue.

  This function always returns the current size or
  the current number of entries in the queue, excluding
  the just-removed entry
 */
extern size_t
mtqRm( mtq_t* q, mtq_entry_t* qe )
{
	size_t				qCurrSz = 0;
	mtq_entry_t*			removedEntry;

	pthread_mutex_lock( &q->lock );

		/*
		  Wait for material to remove
		 */
		while ( q->qHead == NULL )
		{
			pthread_cond_wait( &q->cv, &q->lock );
		}

		/*
		  Copy the data out
		 */
		*qe = *( q->qHead );

		/*
		  Clean ut up
		 */
		qe->next = NULL;

		/*
		  Remove the head entry of the queue
		 */
		removedEntry = q->qHead;
		q->qHead = q->qHead->next;
		if ( q->qHead == NULL )
		{
			q->qTail == NULL;
		}

		qCurrSz = --( q->qCurrSize );

		/*
		  Return this entry to the free memory store
		 */
		storeFree_nl( q, removedEntry );

		/*
		  Let the threads who are blocked in the
		  mtqAdd() call know that there is room
		  in the queue that is available for the
		  addition of new entries
		 */
		pthread_cond_signal( &q->cv );

	pthread_mutex_unlock( &q->lock );

	return qCurrSz;
}


/*
  The current size of a queue is interesting.
  Here we define the "current size of a queue"
  as the number of entries in a queue at the
  moment of inquiry
 */
extern size_t
mtqCurrentSize( mtq_t* q )
{
	size_t			qsz = 0;

	pthread_mutex_lock( &q->lock );

		qsz = q->qCurrSize;

	pthread_mutex_unlock( &q->lock );

	return qsz;
}


extern void
mtqPrint( mtq_t* q )
{
	mtq_entry_t*		qe;

	pthread_mutex_lock( &q->lock );

		qe = q->qHead;
		while ( qe != NULL )
		{
			printf( "Queue entry at %p: %c\n",
				qe, qe->data );
			qe = qe->next;
		}

	pthread_mutex_unlock( &q->lock );
}

extern int
main( int argc, char* argv[] )
{
	mtq_t			q;
	mtq_entry_t		qe;
	size_t			qSz = 11;

	mtqInit( &q, qSz );
	printf( "Queue after creation:\n" );
	mtqPrint( &q );

	mtqAdd( &q, 'a' );
	printf( "Queue after an addition:\n" );
	mtqPrint( &q );

	mtqAdd( &q, 'b' );
	printf( "Queue after an addition:\n" );
	mtqPrint( &q );

	mtqAdd( &q, 'c' );
	printf( "Queue after an addition:\n" );
	mtqPrint( &q );

	mtqAdd( &q, 'd' );
	printf( "Queue after an addition:\n" );
	mtqPrint( &q );

	mtqRm( &q, &qe );
	printf( "Removed queue entry: %c\n",
		qe.data );
	printf( "Queue after a removal:\n" );
	mtqPrint( &q );

	return 0;
}
