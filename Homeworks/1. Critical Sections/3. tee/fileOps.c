#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
  
long length;
char* buffer = 0;
FILE *fileToRead, *fileToWrite;
pthread_cond_t go;
pthread_mutex_t lock;

void* writeToFile() {

  pthread_cond_wait(&go, &lock);
  fputs(buffer, fileToWrite);

}

void *printSTD() {

  pthread_cond_wait(&go, &lock);
  for (int i = 0; i < length; i++) {
      printf("%c", buffer[i]);
    }
  printf("\n");
}

void *readFile() {

    pthread_mutex_lock(&lock);
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

  pthread_cond_broadcast(&go);
  pthread_mutex_unlock(&lock);

}

int main(int argc, char *argv[])
{
    fileToRead = fopen (argv[1], "rb"); //was "rb"
    fileToWrite = fopen(argv[2], "w");
    pthread_cond_init(&go, NULL);
    pthread_mutex_init(&lock, NULL);

    pthread_t print, write, read;

    printf("Attempting to write to file\n");

    pthread_create(&read, NULL, readFile, NULL);
    pthread_create(&print, NULL, printSTD, NULL);
    pthread_create(&write, NULL, writeToFile, NULL);

    pthread_join(read, NULL);
    pthread_join(print, NULL);
    pthread_join(write, NULL);
    
    pthread_exit(NULL);
  
}