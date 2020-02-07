/*
  finding palindromic words in a dictionary using openmp

    usage with gcc (version 4.2 or higher required):
      gcc -O -fopenmp -o palindromic palindromic.c
      ./palindromic size numThreads
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAXWORKERS 24
#define WORDLENGTH 40
#define TASKLENGTH 10
#define MAXSIZE 30000
#define MAX_THREADS 12 

int sum = 0;
int size;
char dictionary[MAXSIZE][WORDLENGTH];
int result_buffer[MAXSIZE];
FILE* results;
FILE* fileToRead;
  

/*
    reverse string
    char * word, string to reverse
*/
void reverse(char * word, char * r){
  int i, j;
  i = 0;
  j = strlen(word) - 1;
  while (j >= 0) {
    r[i++] = word[j--];
  }
  r[i]  = '\0';
}

/*
  binarySearch for searching in the dictionary
*/
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

void Workers(int size){
  int i, result;
  #pragma omp parallel for reduction (+:sum)
  for (i = 0; i < size; i++) {
    // 1. Get word
    char * word = dictionary[i];
    // 2. flip word
    char flip[WORDLENGTH];
    reverse(word, flip);
    // 3. search for fliped word in dictionary
    result = binarySearch(i, size -1, flip);
    // 4. if in dictionary, write to result file 
    if(result != -1){
      fprintf(results, "%s\n", dictionary[i]);
      sum++;
    }
  }
}

int main(int argc, char *argv[]){
  double start_time, end_time;
  int k = 0, i, numThreads;
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


  // Read dictionary
  while(fscanf(fileToRead,"%s",dictionary[k]) == 1){
    for(i= 0; dictionary[k][i]; i++){
        dictionary[k][i] = tolower(dictionary[k][i]);
    }
    result_buffer[k] = 0;
    k++;
  }
  fclose(fileToRead);
  size = k - 1;

  if(argc > 1)
  printf("\nSearching for palindromic words in %s . . . . .\n", argv[1]);

  start_time = omp_get_wtime();                  // Start time for benchmark
  Workers(size);
  end_time = omp_get_wtime();                    // End time for benchmar

  fclose(results);
  
  printf("The execution took %g ms to complete\n", (end_time - start_time)*1000);
  printf("Number of palindromic words found: %d\n", sum);
  printf("Result stored in results.txt\n");

}