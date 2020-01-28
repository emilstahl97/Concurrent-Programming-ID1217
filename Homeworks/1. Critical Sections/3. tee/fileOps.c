#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
  
long length;
char* buffer = 0;
FILE *fileToRead, *fileToWrite;


void* writeToFile() {

  fputs(buffer, fileToWrite);

}

void *printSTD() {

  for (int i = 0; i < length; i++) {
      printf("%c", buffer[i]);
    }
  printf("\n");
}

void *readFile() {

    if (fileToRead)
    {
      fseek (fileToRead, 0, SEEK_END);
      length = ftell (fileToRead);
      fseek (fileToRead, 0, SEEK_SET);
      buffer = (char*)malloc ((length+1)*sizeof(char));
      if (buffer)
      {
        fread (buffer, sizeof(char), length, fileToRead);
      }
      fclose (fileToRead);
    }
    buffer[length] = '\0';

}

int main(int argc, char *argv[])
{
    fileToRead = fopen (argv[1], "rb"); //was "rb"
    fileToWrite = fopen(argv[2], "w");

    pthread_t print, write, read;

    printf("Attempting to write to file\n");

    pthread_create(&read, NULL, readFile, NULL);
    sleep(1);
    pthread_create(&print, NULL, printSTD, NULL);
    pthread_create(&write, NULL, writeToFile, NULL);

    pthread_join(read, NULL);
    pthread_join(print, NULL);
    pthread_join(write, NULL);
    
    pthread_exit(NULL);
  
}