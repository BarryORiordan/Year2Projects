/*  This program reads the contents of a file "file.txt" and sends them over a pipe. 
    The program also prints out the contents of file.txt to standard out. However, 
    the purpose of this program is to provide skeleton code for the students to modify
    and implement a file copying program in a file named 'copyFile.c' that takes command
    line arguments to select whether to encrypt the input data, and the names of the input 
    and output files. */   

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
  FILE *from;

  /* File descriptor array to hold two ends of the pipe */
  int pfd[2];

  pid_t pid;


  /* Add code to check for -f parameter */


  /* Add code to read in the names for input and output files */


  /* Sample code that only reads from file.txt. Modify it. */ 
  from = fopen("file.txt", "r");
  if (from == NULL)
  {   

        /* CHECK this segment of code by running this program when there is no "file.txt" in the current directory */
        /* Then modify this code to print out the missing file as specified by the command line parameters. */ 
        /* NOTE: also write this code for failing to create an output file as requested by the user. */
	perror("file.txt");
	return 1;
  }

  /* Add error checking code as above, to check if output file opens without returing NULL */


  // create pipe
  if (pipe(pfd) == -1) {
	fprintf(stderr,"Pipe failed");

	/* close ALL the open files */
	fclose(from);


	return 1;
  }

  /* now create the processes */
  pid = fork();

  if (pid < 0) { /* error occurred */
	fprintf(stderr, "Fork Failed");

	/* close ALL the open files */
	fclose(from);

	return 1;
  }
  if (pid > 0) {  /* parent */
	/* close the end of the pipe we do not need */
	close(pfd[READ_END]);

	/* read from the input file and write to the pipe */
	while ((c = getc(from)) != EOF){
 			/* Add code to flip c before outputting to the pipe */
 

			/* We pass &c i.e. the address of c. See definition of 'write' at:
			http://pubs.opengroup.org/onlinepubs/009695399/functions/write.html  */
			write(pfd[WRITE_END],&c, 1);

			/* Only to show that file is being read. 
                         REMOVE this code before submitting copyFile.c */
			putchar(c);

	}

	/* close the input file and the write end of the pipe */
	fclose(from);
	close(pfd[WRITE_END]);

	wait(NULL);
  }
  else{  /* child process */

         /* ADD code to close the end of the pipe we do not need */



         /* Do we need to close the input file in the child process even though the child process does nothing with it? 
            Note that a successful fclose returns 0. Print out 
            the return value of fclose to confirm you are not closing an already closed file.
	    e.g. 

              int w=fclose(from);
              printf("w = %d \n", w);

            Notice, the return value may vary from one execution to the next because concurrent processes
            are not guaranteed to run in the same order each time.

	    Research Question: What if the child process closes the file with the above call to fclose when the 
            parent is still reading from it? 
            
            Answer: First look up "copy on write in fork", and then read 
            http://unix.stackexchange.com/questions/91058/file-descriptor-and-fork 
         */




         /* Add code to read from the READ_END of the pipe and write to the output file.
            Then decide where to call fclose on output file and close the  */


  }

  return 0;
}
