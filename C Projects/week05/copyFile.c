#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{
    char c;
    FILE *from, *to;
    char *origFile, *destFile;

    int pfd[2];                                   //File descriptor array to hold two ends of the pipe.
    pid_t pid;

    int flipping = (strcmp(argv[1], "-f") == 0);  //Check for -f parameter.
    if(flipping)                                  //Code to read in the names for input and output files.
    {
        origFile = argv[2];
        destFile = argv[3];
    }
    else
    {
        origFile = argv[1];
        destFile = argv[2];
    }

    from = fopen(origFile, "r");                  //Code that reads from file.txt.
    if(from == NULL)                              //Checking to see if "from" opens without returing NULL.
    {
        perror(origFile);
        return 1;
    }

    to = fopen(destFile, "w");                    //Code that reads from copy.txt.
    if(to == NULL)                                //Checking to see if "to" opens without returing NULL.
    {
        perror(destFile);
        return 1;
    }

    if(pipe(pfd) == -1)                           //Creating the pipe.
    {
        fprintf(stderr,"Pipe failed");

        fclose(from);                             //Closing all open files.
        fclose(to);
        return 1;
    }

    pid = fork();                                 //Creating the processes.
    if(pid < 0)                                   //Error occurred.
    {
        fprintf(stderr, "Fork Failed");           //"Fork Failed" is output.

        fclose(from);                             //Closing all open files.
        fclose(to);
        return 1;
    }

    if(pid > 0)                                   //Parent process.
    {
        close(pfd[READ_END]);                     //Closed the end of pipe not needed.
        while((c = getc(from)) != EOF)            //Read from the input file and write to the pipe.
        {
            if(flipping)                          //Code to flip c before outputting to the pipe.
                c = flipChar(c);

			write(pfd[WRITE_END], &c, 1);         //Write the passed address of c to WRITE_END.
        }
        fclose(from);                             //Closing the input file.
        close(pfd[WRITE_END]);                    //Closing the WRITE_END of the pipe.
        wait(NULL);                               //Waiting for the child to finish.
    }

    else                                          //Child process.
    {
        close(pfd[WRITE_END]);                    //Close the end of pipe not needed.

        while(read(pfd[READ_END], &c, 1))         //Read from the READ_END of the pipe.
            putc(c, to);                          //Write to the output file.

        fclose(to);                               //Closing the output file.
        close(pfd[READ_END]);                     //Closing the READ_END of the pipe.
                                                  //No "wait(NULL);" as the child does not need to wait for the parent to finish.
    }
    return 0;
}
