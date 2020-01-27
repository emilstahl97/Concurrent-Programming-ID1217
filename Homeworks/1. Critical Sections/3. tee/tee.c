/* gcc unixtee.c -o unixtee -lpthread
      ./unixtee file.txt createNewFileName */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


FILE *fp1, *fp2;
int *buffer, bufferSize;

/*write to file tee.txt*/
void* writeFile(int buffer){
  /*writes a character in the file*/
  fputc(buffer, fp2);
  return NULL;
}

/*write from buffer to standard outputt*/
void* printText(int buffer){
  printf("%c", buffer);
  return NULL;
}

void print(int buffer[], int length) {
	int i;
	printf("> [");
	for (i = 0; i < length; i++)
		printf(" %d", buffer[i]);
	printf(" ]\n");
}

int main(int argc, char *argv[]) {

  FILE *f1, *f2;
  int data;
  f1 = fopen(argv[1], "r");
  f2 = fopen(argv[2], "w");

  while (fscanf(f1, "%d", &data) != EOF) {
		    bufferSize++;
		    buffer = (int *) realloc(buffer, bufferSize * sizeof(int));
		    buffer[bufferSize - 1] = data;
	    }
      print(buffer, bufferSize);
	    fclose(f1);
	    printf("%d elements read\n", bufferSize); 

}