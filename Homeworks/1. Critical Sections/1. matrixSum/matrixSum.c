/* matrix summation using pthreads

   features: uses mutex locks to prevent race conditions in critical sections; 
             each worker/thread is given a row in the matrix to compute
             the global struct is then updated with the results by locking the section that is to be updated
             when all workers/threads are done the results are printed by the main method.

   usage under Linux:
            gcc matrixSum.c -o matrixSum -lpthread
            ./matrixSum size numWorkers range
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
#define SECTION 1
#define DEBUGS 
//#define DEBUG       // Uncomment to get debugging printouts

struct worker {
  int min;                    // Holds the minimum element
  int max;                    // Holds the maximum element
  int total;                  // Holds the total sum of all elements
  int minIndex[2];            // Array to hold the indexes of the minimum element
  int maxIndex[2];            // Array to hold the indexes of the maximum element
} element;

pthread_mutex_t minLock;      // mutex lock to update the minimum element
pthread_mutex_t maxLock;      // mutex lock to update the maximum element
pthread_mutex_t rowLock;      // mutex lock to update the current row
pthread_mutex_t totalLock;    // mutex lock to update the total sum
int minIndex[2];              // Array to hold the indexes of the minimum element
int maxIndex[2];              // Array to hold the indexes of the maximum element
int size, stripSize;          // assume size is multiple of numWorkers
int row = 0;                  // Variable to specify row
int numWorkers;               // number of workers
int matrix[MAXSIZE][MAXSIZE]; // Array to represent matrix
double start_time, end_time;  // start and end times 

void *Worker(void *);

/* timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized ) {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  element.min = 1000;
  element.max = 0;
  element.total = 0;
  int i, j, range;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutexes */
  pthread_mutex_init(&minLock, NULL);
  pthread_mutex_init(&maxLock, NULL);
  pthread_mutex_init(&rowLock, NULL);
  pthread_mutex_init(&totalLock, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  range = (argc > 3) ? atoi(argv[3]) : 100;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;

  /* initialize the matrix */
  srand(time(NULL)); //seed the generator
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
  for (l = 0; l < numWorkers; l++) {
    pthread_create(&workerid[l], &attr, Worker, (void *) l);
  }

  for(int i = 0; i < numWorkers; i++) {
    pthread_join(workerid[i], NULL);
  }

  end_time = read_timer();
  printf("The execution took %g ms to complete\n", (end_time - start_time)*1000);
  printf("The total sum of all the elements is %d\n", element.total);
  printf("The maximum element is %d at position [%d,%d]\n", element.max, element.maxIndex[1]+1,element.maxIndex[0]+1);
  printf("The minimum element is %d at position [%d,%d]\n", element.min, element.minIndex[1]+1,element.minIndex[0]+1);
  
  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix. */
void *Worker(void *arg) {
  long myid = (long) arg;
  int localTotal, i, j, first, last;

  #ifdef DEBUG
    printf("worker %ld has started\n", myid);
  #endif

  localTotal = 0;
  while(row < size){
  if(row < size){
    pthread_mutex_lock(&rowLock);
    first = row;
    row++;
    last = row;
    pthread_mutex_unlock(&rowLock);

    #ifdef DEBUG
      printf("row %d computed by thread %ld\n", first, myid);
    #endif

    /* sum values in my strip */
    for (i = first; i < last; i++) {

      for (j = 0; j < size; j++) {
        localTotal += matrix[i][j];

        if(matrix[i][j] < element.min){
          pthread_mutex_lock(&minLock);
          if(matrix[i][j] < element.min){
            element.min = matrix[i][j];
            element.minIndex[0] = i;
            element.minIndex[1] = j;
          }
          pthread_mutex_unlock(&minLock);
        } else if(matrix[i][j] > element.max){
          pthread_mutex_lock(&maxLock);
          if(matrix[i][j] > element.max){
            element.max = matrix[i][j];
            element.maxIndex[0] = i;
            element.maxIndex[1] = j;
          }
          pthread_mutex_unlock(&maxLock);
        }
      }
    }
  }
}
  pthread_mutex_lock(&totalLock);
  element.total += localTotal;
  pthread_mutex_unlock(&totalLock);

  return 0;
}