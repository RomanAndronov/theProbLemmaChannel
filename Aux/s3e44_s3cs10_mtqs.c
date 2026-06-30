/*
  This is an auxiliary code for S3E44.
  It implements a simplified version
  of a thread-safe blocking queue based
  on the idea discussed in the above episode.
  "Blocking" means that if such a queue is
  empty then the caller of the mtqRm()
  function will block for as long this queue
  remains empty until a new entry into that
  queue arrives.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct MTQENTRY
{
	char			data;
} mtq_entry_t;

typedef struct MTQ
{
	size_t			qSize;
	size_t			curre;
	size_t			nume;
	mtq_entry_t		*q;
	pthread_mutex_t		lock;
	pthread_cond_t		cv;
} mtq_t;

extern mtq_t*
mtqInit( mtq_t *mtq, size_t qsize )
{
	mtq->qSize = 0;
	mtq->curre = 0;
	mtq->nume = 0;

	mtq->q = ( mtq_entry_t* )calloc( qsize, sizeof( mtq_entry_t ) );
	if ( !mtq->q )
	{
		printf( "calloc( %ld ) failed: %s",
			qsize * sizeof( mtq_entry_t ),
			strerror( errno ) );
		return NULL;
	}

	mtq->qSize = qsize;
	pthread_mutex_init( &mtq->lock, NULL );
	pthread_cond_init( &mtq->cv, NULL );

	return mtq;
}

extern mtq_entry_t*
mtqAdd( mtq_t *mtq, mtq_entry_t *mtqe )
{
	size_t			i;
	mtq_entry_t*		rv = mtqe;

	pthread_mutex_lock( &mtq->lock );

		while ( mtq->nume >= mtq->qSize )
		{
			pthread_cond_wait( &mtq->cv, &mtq->lock );
		}

		i = ( mtq->nume + mtq->curre ) % mtq->qSize;
		mtq->q[ i ] = *mtqe;
		mtq->nume++;
		pthread_cond_broadcast( &mtq->cv );

	pthread_mutex_unlock( &mtq->lock );

	return rv;
}

extern mtq_entry_t*
mtqRm( mtq_t *mtq, mtq_entry_t *mtqe )
{
	mtq_entry_t*		rv = mtqe;

	pthread_mutex_lock( &mtq->lock );

		while ( mtq->nume == 0 )
		{
			pthread_cond_wait( &mtq->cv, &mtq->lock );
		}

		*mtqe = mtq->q[ mtq->curre ];
		mtq->curre = ( mtq->curre + 1 ) % mtq->qSize;
		mtq->nume--;
		pthread_cond_broadcast( &mtq->cv );

	pthread_mutex_unlock( &mtq->lock );

	return rv;
}

extern int
main( int argc, char* argv[] )
{
	mtq_t			q;
	size_t			qSz = 7;
	mtq_entry_t		mtqe;

	mtqInit( &q, qSz );
	mtqe.data = 'a';
	mtqAdd( &q, &mtqe );
	mtqRm( &q, &mtqe );

	return 0;
}
