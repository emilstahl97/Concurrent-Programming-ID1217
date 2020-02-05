/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>
#include <stdio.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

double start_time, end_time;
int omp_get_num_threads(void);

struct worker {
  int min;                    // Holds the minimum element
  int max;                    // Holds the maximum element
  int total;                  // Holds the total sum of all elements
  int minIndex[2];            // Array to hold the indexes of the minimum element
  int maxIndex[2];            // Array to hold the indexes of the maximum element
} element;

omp_lock_t minLock;            // mutex lock to update the minimum element
omp_lock_t maxLock;            // mutex lock to update the maximum element
int minIndex[2];              // Array to hold the indexes of the minimum element
int maxIndex[2];              // Array to hold the indexes of the maximum element
int size, stripSize;          // assume size is multiple of numWorkers
int row = 0;                  // Variable to specify row
int numWorkers;               // number of workers
int matrix[MAXSIZE][MAXSIZE]; // Array to represent matrix
double start_time, end_time;  // start and end times

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, range, total=0;

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  range = (argc > 3) ? atoi(argv[3]) : 100;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);
  
  /* initialize the matrix */
  srand(time(NULL)); //seed the generator
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%range;
	  }
  }

  element.min = matrix[0][0];
  element.max = matrix[0][0];
  element.total = 0;

  /* print the matrix */
  for (i = 0; i < size; i++) {
	  printf("[");
	  for (j = 0; j < size; j++) {
	    printf(" %d ", matrix[i][j]);
	  }
	  printf("]\n");
  }

  omp_init_lock(&minLock);
  omp_init_lock(&minLock);


start_time = omp_get_wtime();
#pragma omp parallel 
{
  #pragma for reduction (+:total) private(j) 
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++){
      if(matrix[i][j] < element.min) {
        #pragma omp critical(min)
        {
          if(matrix[i][j] < element.min) {
            element.min = matrix[i][j];
            element.minIndex[0] = i;
            element.minIndex[1] = j; 
          }
        }
      }
      if(matrix[i][j] > element.max) {
        #pragma omp critical(max)
        {
          if(matrix[i][j] > element.max) {
            element.max = matrix[i][j];
            element.maxIndex[0] = i;
            element.maxIndex[1] = j; 
          }
        }
      }
    
      element.total += matrix[i][j];
    }
}
      #pragma omp single
      printf("Number of threads executing is %d \n", omp_get_num_threads());
}

// implicit barrier

  end_time = omp_get_wtime();

  #pragma omp master

  printf("The execution took %g ms to complete\n", (end_time - start_time)*1000);
  printf("The total sum of all the elements is %d\n", element.total);
  printf("The minimum element is %d at position [%d,%d]\n", element.min, element.minIndex[1]+1,element.minIndex[0]+1);
  printf("The maximum element is %d at position [%d,%d]\n", element.max, element.maxIndex[1]+1,element.maxIndex[0]+1);
}

