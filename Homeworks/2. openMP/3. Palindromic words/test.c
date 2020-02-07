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
int mark[MAXSIZE];

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
  int i;
  #pragma omp parallel for reduction (+:sum)
  for (i = 0; i < size; i++) {
    // 1. Get word
    char * word = dictionary[i];
    // 2. flip word
    char flip[WORDLENGTH];
    reverse(word, flip);
    // 3. search for fliped word in dictionary
    int result = binarySearch(i, size -1, flip);
    // 4. mark if in dictionary
    if(result != -1){
      sum++;
      mark[i] = 1;
    }
  }
}

int main(int argc, char *argv[]){
  double start_time, end_time;
  int k = 0, i, numThreads;
  long l;
  FILE* fileToRead;
  FILE *results;

  if(argc < 3){
    printf("Error! Argument missing: file to examine\n");
    exit(0);
  }

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
    mark[k] = 0;
    k++;
  }
  fclose(fileToRead);
  size = k - 1;

  start_time = omp_get_wtime();                  // Start time for benchmark
  Workers(size);
  end_time = omp_get_wtime();                    // End time for benchmark

  for(i = 0; i < size; i++){
    if(mark[i]){
      fprintf(results, "%s\n", dictionary[i]);
    }
  }
  printf("Result stored in %s\n", argv[2]);

  // Print execution time
  printf("Num threads: %d. The execution time is %g sec. Num words: %d\n"
          , numThreads, end_time - start_time, sum);

}