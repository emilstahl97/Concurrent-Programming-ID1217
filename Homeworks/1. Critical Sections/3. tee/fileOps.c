#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
  
long length;
int result;
char* fileName;
char* buffer = 0;
FILE *fileToRead, *fileToWrite;
pthread_cond_t go;
pthread_mutex_t lock;

void* writeToFile() {

  pthread_cond_wait(&go, &lock);
  result = fputs(buffer, fileToWrite);

  return 0;
}

void *printSTD() {

  pthread_cond_wait(&go, &lock);
  for (int i = 0; i < length; i++) {
      printf("%c", buffer[i]);
    }
  printf("\n");

  return 0;
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
    }
    buffer[length] = '\0';

  pthread_cond_broadcast(&go);
  pthread_mutex_unlock(&lock);

  return 0;

}

int main(int argc, char *argv[])
{
    fileToRead = fopen (argv[1], "rb"); //was "rb"
    fileToWrite = fopen(argv[2], "w");
    fileName = argv[2];
    pthread_cond_init(&go, NULL);
    pthread_mutex_init(&lock, NULL);

    pthread_t print, write, read;

    printf("Attempting to write to file...\n");

    pthread_create(&read, NULL, readFile, NULL);
    pthread_create(&print, NULL, printSTD, NULL);
    pthread_create(&write, NULL, writeToFile, NULL);

    pthread_join(read, NULL);
    pthread_join(print, NULL);
    pthread_join(write, NULL);
    
    fclose (fileToRead);
    fclose(fileToWrite);

    if(result == EOF) {
    printf("Write to file %s failed \n", fileName);
  }
  else {
    printf("Write to file %s succeded \n", fileName);
  }
    
  pthread_exit(NULL);
  
}