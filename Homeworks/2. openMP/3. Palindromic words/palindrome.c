/* palindromic word finder using openMP 

   features: find all palindromic words in a input file and writes the results to a specified result file

   usage under Linux:

            gcc palindrome.c -o palindrome -fopenmp
            ./palindrome <input file> <output file> numberOfThreads
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <omp.h>

#define WORD_LENGTH 35
#define MAX_SIZE 400000
#define MAX_THREADS 36 

int threads, sum = 0;
int result_buffer[MAX_SIZE];
char dictionary[MAX_SIZE][WORD_LENGTH];
FILE* fileToRead;
FILE* results;
  
int binarySearch(int l, int r, char * x){
  while (l <= r){
    int m = l + (r-l)/2;
    int result = (int)strcmp(x, dictionary[m]);
    if (result == 0)
      return m;
    if (result > 0)
      l = m + 1;
    else
      r = m - 1;
  }
  return -1;
}

void reverse(char * word, char * r){
  int i, j;
  i = 0;
  j = strlen(word) - 1;
  while (j >= 0) {
    r[i++] = word[j--];
  }
  r[i]  = '\0';
}

void Worker(int size){
  int i, result;
  #pragma omp parallel 
  {
  #pragma omp for reduction (+:sum)
  for (i = 0; i < size; i++) {

    char* word = dictionary[i];
    char flip[WORD_LENGTH];
    reverse(word, flip);
    result = binarySearch(i, size-1, flip);
    if(result != -1) {
        #pragma omp critical 
        {
        result_buffer[i] = 1;
        sum++;
        }
    }
  }
    #pragma omp single 
    {
    threads = omp_get_num_threads();
    }
  }
}

int main(int argc, char *argv[]){
  double start_time, end_time;
  int numThreads, i, j = 0;
  
  fileToRead = (argc > 1) ? fopen(argv[1], "r+") : fopen("./words.txt", "r+");
  results = (argc > 2) ? fopen(argv[2], "w+") : fopen("./results.txt", "w+");
  numThreads = (argc > 3) ? atoi(argv[3]) : MAX_THREADS;
  
  omp_set_num_threads(numThreads);
  
  /* No file */
  if (!fileToRead || !results) {
      fprintf(stderr, "404 File not Found\n");
      exit(1);
    }

  // Read input file into dictionary char buffer
  while(fscanf(fileToRead, "%s", dictionary[j]) == 1) {
    for(i= 0; dictionary[j][i]; i++) {
        dictionary[j][i] = tolower(dictionary[j][i]);
    }
    result_buffer[j] = 0;
    j++;
  }
  fclose(fileToRead);

  if(argc > 1)
  printf("\nSearching for palindromic words in %s . . . . .\n", argv[1]);

  start_time = omp_get_wtime(); 
  Worker(j-1);
  end_time = omp_get_wtime();  

  for(i = 0; i < (j-1); i++) {
    if(result_buffer[i] == 1)
      fprintf(results, "%s\n", dictionary[i]);
  }
  fclose(results);

  printf("Number of executing threads: %d\n", threads);
  printf("The execution took %g ms to complete\n", (end_time - start_time)*1000);
  printf("Number of palindromic words found: %d\n", sum);
  if(argc > 2)
  printf("Result stored in %s\n", argv[2]);
  else 
  printf("Result stored in results.txt");
}