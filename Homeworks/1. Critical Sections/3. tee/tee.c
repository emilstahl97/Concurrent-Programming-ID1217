/* gcc unixtee.c -o unixtee -lpthread
      ./unixtee file.txt createNewFileName */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#define MAXSIZE 100000;

int buffer, bufferSize;

FILE *fp1, *fp2;
/*
//write to file tee.txt
void* writeFile(){
  //writes a character in the file
  fputc(buffer, fp2);
  return NULL;
}

//write from buffer to standard outputt
void print(void *struc) {
	int i;
	for (i = 0; i < bufferSize; i++)
		printf("%d ", buffer[i]);
	  printf("\n");
}
*/
int main(int argc, char *argv[]) {

  FILE *f1, *f2;
  int data;
  pthread_t th1;
  f1 = fopen(argv[1], "r");
  f2 = fopen(argv[2], "w");

  while(!feof(fp1)) {

    /*Read from the file and save it in the buffer*/
    buffer = fgetc(fp1);
    bufferSize++;
  }

  printf("buffer = %d\n", bufferSize);
}