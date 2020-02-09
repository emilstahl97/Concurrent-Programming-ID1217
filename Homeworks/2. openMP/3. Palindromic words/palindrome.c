
/* palindromic word finder using openMP 

   features: find all palindromic words in a input file and writes the results to a specified result file

   usage under Linux:

            gcc palindrome.c -o palindrome -fopenmp
            ./palindrome <input file> <output file> numThreads
*/


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define WORD_LENGTH 40
#define MAX_SIZE 30000
#define MAX_THREADS 24 

int sum = 0;
int result_buffer[MAX_SIZE];
char dictionary[MAX_SIZE][WORD_LENGTH];
FILE* fileToRead;
FILE* results;
  
void reverse(char * word, char * r){
  int i, j;
  i = 0;
  j = strlen(word) - 1;
  while (j >= 0) {
    r[i++] = word[j--];
  }
  r[i]  = '\0';
}

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

void Worker(int size){
  int i, result;
  #pragma omp parallel for reduction (+:sum)
  for (i = 0; i < size; i++) {
    // 1. Get word
    char * word = dictionary[i];
    // 2. flip word
    char flip[WORD_LENGTH];
    reverse(word, flip);
    // 3. search for fliped word in dictionary
    result = binarySearch(i, size -1, flip);
    // 4. if in dictionary, write to result file 
    if(result != -1) {
        #pragma omp critical 
        {
        fprintf(results, "%s\n", dictionary[i]);
        sum++;
        }
    }
  }
}

int main(int argc, char *argv[]){
  double start_time, end_time;
  int numThreads, i, j = 0;
  long l;
  
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

  fclose(results);
  
  printf("The execution took %g ms to complete\n", (end_time - start_time)*1000);
  printf("Number of palindromic words found: %d\n", sum);
  printf("Result stored in results.txt\n");
}