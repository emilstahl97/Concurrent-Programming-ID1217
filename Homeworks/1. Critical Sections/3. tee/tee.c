/* gcc unixtee.c -o unixtee -lpthread
      ./unixtee file.txt createNewFileName */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


FILE *fp1, *fp2;
int *buffer, bufferSize;
struct file {

  int *buffer; 
  int bufferSize;

} file;

/*write to file tee.txt*/
void* writeFile(int buffer){
  /*writes a character in the file*/
  fputc(buffer, fp2);
  return NULL;
}

/*write from buffer to standard outputt*/
void print(void *struc) {
	int i;
	for (i = 0; i < bufferSize; i++)
		printf("%d ", buffer[i]);
	  printf("\n");
}

int main(int argc, char *argv[]) {

  FILE *f1, *f2;
  int data;
  pthread_t th1;
  struct file file;
  f1 = fopen(argv[1], "r");
  f2 = fopen(argv[2], "w");

  while (fscanf(f1, "%d", &data) != EOF) {
		    bufferSize++;
		    buffer = (int *) realloc(buffer, bufferSize * sizeof(int));
		    buffer[bufferSize - 1] = data;
	    }
	    fclose(f1);

          pthread_create(&th1, NULL, print, NULL);

          pthread_join(th1, NULL);

          pthread_exit(NULL);

}