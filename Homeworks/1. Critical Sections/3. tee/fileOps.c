#include <stdlib.h>
#include <stdio.h>
  
long length;
char* buffer = 0;
FILE *fileRead, *fileWrite;


void* writeToFile() {

  fputs(buffer, fileWrite);

}

void *printSTD() {

  for (int i = 0; i < length; i++) {
      printf("%c", buffer[i]);
    }
  printf("\n");
}

void *readFile() {

    if (fileRead)
    {
      fseek (fileRead, 0, SEEK_END);
      length = ftell (fileRead);
      fseek (fileRead, 0, SEEK_SET);
      buffer = (char*)malloc ((length+1)*sizeof(char));
      if (buffer)
      {
        fread (buffer, sizeof(char), length, fileRead);
      }
      fclose (fileRead);
    }
    buffer[length] = '\0';

}

int main(int argc, char *argv[])
{
    fileRead = fopen (argv[1], "rb"); //was "rb"
    fileWrite = fopen(argv[2], "w");

    printf("Attempting to write to file\n");
  
    readFile();
    writeToFile();
    printSTD();
}