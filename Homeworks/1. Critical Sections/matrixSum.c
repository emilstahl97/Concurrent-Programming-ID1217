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
#define DEBUGS 
#define DEBUG
#define SECTION 1

struct worker
{
  int max;
  int maxIndex[2];
  int min;
  int minIndex[2];
  int total;
} data;

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_mutex_t minlock;
pthread_mutex_t maxlock;
pthread_mutex_t rowlock;
pthread_mutex_t sumlock;
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */
int minIndex[2];
int maxIndex[2];
int row = 0;
int sum;

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

double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
int sums[MAXWORKERS]; /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  data.min = 1000;
  data.max = 0;
  data.total = 0;
  int i, j, range;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_mutex_init(&minlock, NULL);
  pthread_mutex_init(&maxlock, NULL);
  pthread_mutex_init(&rowlock, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  range = (argc > 3) ? atoi(argv[3]) : 100;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;
  /* initialize the matrix */
  srand(time(NULL));
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%range;
	  }
  }
  /* print the matrix */
  for (i = 0; i < size; i++) {
	  printf("[");
	  for (j = 0; j < size; j++) {
	    printf(" %d ", matrix[i][j]);
	  }
	  printf("]\n");
  }

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *) l);

  for(int i = 0; i < numWorkers; i++){
    pthread_join(workerid[i], NULL);
  }

  end_time = read_timer();
  printf("The execution time is %g sec\n", end_time - start_time);
  printf("The total is %d\n", data.total);
  printf("The max is %d at position (%d,%d)\n", data.max, data.maxIndex[1]+1,data.maxIndex[0]+1);
  printf("The min is %d at position (%d,%d)\n", data.min, data.minIndex[1]+1,data.minIndex[0]+1);
  

  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  long myid = (long) arg;
  int localTotal, i, j, first, last;

#ifdef DEBUG
  printf("worker %ld has started\n", myid);
#endif

  localTotal = 0;
  while(row < size){
  if(row < size){
    pthread_mutex_lock(&rowlock);
    first = row;
    row++;
    last = row;

    pthread_mutex_unlock(&rowlock);
    #ifdef DEBUG
      printf("row %d computed by thread %ld\n", first, myid);
    #endif
    /* sum values in my strip */

    for (i = first; i < last; i++)

      for (j = 0; j < size; j++){
        localTotal += matrix[i][j];

        if(matrix[i][j] < data.min){
          pthread_mutex_lock(&minlock);
          if(matrix[i][j] < data.min){
            data.min = matrix[i][j];
            data.minIndex[0] = i;
            data.minIndex[1] = j;
          }
          pthread_mutex_unlock(&minlock);
        } else if(matrix[i][j] > data.max){
          pthread_mutex_lock(&maxlock);
          if(matrix[i][j] > data.max){
            data.max = matrix[i][j];
            data.maxIndex[0] = i;
            data.maxIndex[1] = j;
          }
          pthread_mutex_unlock(&maxlock);
        }
      }
  }
}
  pthread_mutex_lock(&sumlock);
  data.total += localTotal;
  pthread_mutex_unlock(&sumlock);

  return 0;
}