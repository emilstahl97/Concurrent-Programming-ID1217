/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
   gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
   ./matrixSum-openmp size numWorkers

   Make run will compile and run the program.

   Size can be specified through SIZE and number of threads
   can be specified through NWORKERS.

*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#define MAXSIZE 30000   /* Maximum matrix size. */
#define MAXWORKERS 8    /* Maximum number of workers. */
#define MAXNR 100       /* Maximum number (- 1) in the array. */
#define MINNR 0         /* Minumum number in the array. */

double start_time, end_time; /* Timestamps. */
int numWorkers;         /* Number of workers used. */
int size;               /* Size of the matrix used. */
int *matrix;            /* The matrix, implemented as a vector. */

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
    /* Initialize the ints used. */
    int i, j, total=0, min=MAXNR, max=MINNR -1, min_y=-1, min_x=-1, max_y=-1, max_x=-1;

    /* Read command line args if any */
    size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
    numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
    if (size > MAXSIZE) size = MAXSIZE;
    if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

    /* Sets the number of threads used. */
    omp_set_num_threads(numWorkers);

    /* Allocates the matrix on the heap. */
    matrix = malloc(sizeof(int) * size * size);

    srand(time(NULL)); /* Seed the randomizer. */
    /* Initialize the matrix. */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            matrix[i*size+j] = (rand() % (MAXNR - MINNR)) + MINNR;
        }
    }

    start_time = omp_get_wtime(); /* Reads the timer just before parallel part. */
#pragma omp parallel for reduction (+:total) private(j)
    for (i = 0; i < size; i++) /* Iterate over every row. */
    {
        for (j = 0; j < size; j++) /* Every column. */
        {
            total += matrix[i*size+j]; /* Update total. */
            if(matrix[i*size+j] < min) /* Checks if min needs to be updated. */
            {
#pragma omp critical(update_min) /* Create critical section. */
                {
                    if(matrix[i*size+j] < min) /* Check once more (another thread could've
                                                  updated the min value. */
                    {
                        min = matrix[i*size+j];
                        min_x = j;
                        min_y = i;
                    }    
                }
            }
            if(matrix[i*size+j] > max) /* Checks if max needs to be updated. */
            {
#pragma omp critical(update_max) /* Create another critical section (with different locks). */
                {
                    if(matrix[i*size+j] > max) /* Check once more for the same reasoning as above. */
                    {
                        max = matrix[i*size+j];
                        max_x = j;
                        max_y = i;
                    }
                }
            }
        }
    }
    /* Implicit barrier. */

    end_time = omp_get_wtime(); /* Reads the timer after the parallel part. */

    free(matrix); /* Frees the matrix. */

    /* Prints the results and the time. */
    //printf("The total is %d.\n", total);
   //printf("Maximum is %d, and is located at %d,%d.\n", max, max_y, max_x);
 //   printf("Minimum is %d, and is located at %d,%d.\n", min, min_y, min_x);
 printf("%g ms\n", (end_time - start_time)*1000);

    return(EXIT_SUCCESS); /* If omitted error codes 23, 26 or 27 are returned to make. */
}