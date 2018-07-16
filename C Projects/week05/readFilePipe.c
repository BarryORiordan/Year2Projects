#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char flipChar(char c);

int main(int argc, char* argv[])
{
    char c;
    FILE *from, *to;
    int flipping = (strcmp(argv[1], "-f") == 0);
    if(flipping)                    //-f flag given
    {
        from = fopen(argv[2], "r");
        if(from == NULL)
        {
            perror(argv[2]);       //If file.txt does not exist, "file.txt" is output
            exit(1);
        }
        to = fopen(argv[3], "w");
        if(to == NULL)
        {
            perror(argv[3]);           //If copy.txt does not exist, "copy.txt" is output
            exit(1);
        }
        while((c = getc(from)) != EOF)
            putc(flipChar(c), to);     //Writes the flipped char c to copy.txt
    }
    else
    {
        from = fopen(argv[1], "r");
        if(from == NULL)
        {
            perror(argv[1]);        //If file.txt does not exist, "file.txt" is output
            exit(1);
        }
        to = fopen(argv[2], "w");
        if(to == NULL)
        {
            perror(argv[2]);        //If copy.txt does not exist, "copy.txt" is output
            exit(1);
        }
        while((c = getc(from)) != EOF)
            putc((c), to);              //Writes the char c to copy.txt
    }

    fclose(from);
    fclose(to);
    exit(0);
}
