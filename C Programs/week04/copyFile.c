#include <stdio.h>
#include <stdlib.h>

int main()
{
  char c;
  FILE *from, *to;

  from = fopen("file.txt", "r");
  if (from == NULL)
  {
    perror("file.txt");             //If file does not exist, "file.txt" is output
    exit(1);
  }

  to = fopen("copy.txt", "w");
  while ((c = getc(from)) != EOF)   //File exists so start writing
    putc(c, to);                    //Writes the char c to copy.txt

  fclose(from);
  fclose(to);
  exit(0);
}
