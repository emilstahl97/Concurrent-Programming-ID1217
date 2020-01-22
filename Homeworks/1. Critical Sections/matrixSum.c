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
#define DEBUGS 1
#define SECTION 1

struct worker
{
  int max;
  int maxPos[2];
  int min;
  int minPos[2];
  int total;
};


pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_mutex_t minlock;
pthread_mutex_t maxlock;
pthread_mutex_t rowlock;
pthread_mutex_t sumlock;
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */
int numArrived = 0;       /* number who have arrived */
int minindex[2];
int maxindex[2];
int row = 0;
int sum;
struct worker data;

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
  int i, j;
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
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;
  //printf("size: %d, #workers: %d\n", size, numWorkers);
  /* initialize the matrix */
  srand(time(NULL));
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%99;
	  }
  }
  minindex[0] = 0;
  minindex[1] = 0;
  maxindex[0] = 0;
  maxindex[1] = 0;

  /* print the matrix */
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *) l);

  for(int i = 0; i < numWorkers; i++){
    pthread_join(workerid[i], NULL);
    //printf("got value %d\n", *(int *)total);
  }
  end_time = read_timer();
  printf("The execution time is %g sec\n", end_time - start_time);
  printf("The total is %d\n", data.total);
  printf("The max is %d at position (%d,%d)\n", data.max, data.maxPos[1]+1,data.maxPos[0]+1);
  printf("The min is %d at position (%d,%d)\n", data.min, data.minPos[1]+1,data.minPos[0]+1);
  

  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  long myid = (long) arg;
  int total, i, j, first, last;

#ifdef DEBUG
  printf("worker %ld (pthread id %ld) has started\n", myid, pthread_self());
#endif


  /* determine first and last rows of my strip */
  //first = myid*stripSize;
  //last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);
  total = 0;
  while(row < size){
  if(row < size){
    pthread_mutex_lock(&rowlock);
    first = row;
    row++;
    last = row;

    pthread_mutex_unlock(&rowlock);
      //printf("row %d computed by thread %ld\n", first, myid);
    /* sum values in my strip */

    for (i = first; i < last; i++)

      for (j = 0; j < size; j++){
        total += matrix[i][j];

        if(matrix[i][j] < data.min){
          pthread_mutex_lock(&minlock);
          if(matrix[i][j] < data.min){
            data.min = matrix[i][j];
            data.minPos[0] = i;
            data.minPos[1] = j;
          }
          pthread_mutex_unlock(&minlock);
        } else if(matrix[i][j] > data.max){
          pthread_mutex_lock(&maxlock);
          if(matrix[i][j] > data.max){
            data.max = matrix[i][j];
            data.maxPos[0] = i;
            data.maxPos[1] = j;
          }
          pthread_mutex_unlock(&maxlock);
        }
      }
  }
  //printf("total: %d, by thread: %ld\n", total, myid);
  //printf("%d\n", total);

}
  pthread_mutex_lock(&sumlock);
  sum += total;
  pthread_mutex_unlock(&sumlock);

  //sums[myid] = total;
  /*Barrier();
  if (myid == 0) {

    total = 0;
    for (i = 0; i < numWorkers; i++)
      total += sums[i];
    /* get end time */

    /* print results */


  //}
  //printf("total: %d\n", total);
}