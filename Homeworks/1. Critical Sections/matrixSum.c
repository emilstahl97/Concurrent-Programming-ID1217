/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_mutex_t maxLock;  /* mutex lock for the max element */
pthread_mutex_t minLock;  /* mutex lock for the min element */
pthread_mutex_t totalLock;  /* mutex lock for the total value */
pthread_mutex_t rowLock;    /* mutex lock for the row */
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */ 
int total = 0;            // 
int nextRow = 0;
int numArrived = 0;       /* number who have arrived */
int size, stripSize;  /* assume size is multiple of numWorkers */
int sums[MAXWORKERS]; /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */
int max[3], min[3];         
double start_time, end_time; /* start and end times */

/* a reusable counter barrier */
void Barrier() {
  pthread_mutex_lock(&barrier);
  numArrived++;
  if (numArrived == numWorkers) {
    numArrived = 0;
    pthread_cond_broadcast(&go);
  } else
    pthread_cond_wait(&go, &barrier);
  pthread_mutex_unlock(&barrier);
}

/* timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = 1;//rand()%99;
	  }
  }

  /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *) l);
  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  int i, row;
  int localTotal = 0; 
  while(nextRow < size) {
    pthread_mutex_lock(&rowLock);
    row = nextRow;
    nextRow++;
    pthread_mutex_unlock(&rowLock);

    for(i = 0; i < size; i++) {
      localTotal = matrix[row][i];
      if(matrix[row][i] < min[0]) {
        pthread_mutex_lock(&minLock);
        min[0] = matrix[row][i];
        min[1] = row;
        min[2] = i;
        pthread_mutex_unlock(&minLock);
      }
      else if(matrix[row][i] > max[0]){
        pthread_mutex_lock(&maxLock);
        max[0] = matrix[row][i];
        max[1] = row;
        max[2] = i;
        pthread_mutex_unlock(&maxLock);
      }
    }
  }
  pthread_mutex_lock(&totalLock);
  total += localTotal;
  pthread_mutex_unlock(&totalLock);

  return 0;
}
