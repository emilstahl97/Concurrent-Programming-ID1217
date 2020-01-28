#include <stdlib.h>
#include <stdio.h>
  
char* buffer = 0;
FILE *f, *f2;


void* writeToFile() {

  int results = fputs(buffer, f2);

}

void *printSTD(int length) {

  for (int i = 0; i < length; i++) {
      printf("%c", buffer[i]);
    }
  printf("\n");
}

int main(int argc, char *argv[])
{
    long length;
    f = fopen (argv[1], "rb"); //was "rb"
    f2 = fopen(argv[2], "w");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char*)malloc ((length+1)*sizeof(char));
      if (buffer)
      {
        fread (buffer, sizeof(char), length, f);
      }
      fclose (f);
    }
    buffer[length] = '\0';

    //printf("buffer = %s\n", buffer);

    printf("Attempting to write to file\n");

    writeToFile();
    printSTD(length);
}