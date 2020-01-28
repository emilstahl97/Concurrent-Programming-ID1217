#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char* buffer = 0;
    long length;
    FILE * f = fopen (argv[1], "rb"); //was "rb"
    FILE * f2 = fopen(argv[2], "w");

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
    for (int i = 0; i < length; i++) {
       printf("%c", buffer[i]);
    }
    printf("\n");
    //printf("buffer = %s\n", buffer);

    printf("Attempting to write to file\n");

    int results = fputs(buffer, f2);

}