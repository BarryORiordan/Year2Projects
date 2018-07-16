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

  int pfd[2];
  pid_t pid;

  int fromInd = 1;

  int flip = (strcmp(argv[1], "-f") == 0);
  if (flip) fromInd++;

  from = fopen(argv[fromInd], "r");
  if (from == NULL)
  {
	perror(argv[1]);
	return 1;
  }

  to = fopen(argv[fromInd+1], "w");
  if (to == NULL)
  {
	perror(argv[2]);
	return 1;
  }

  // create pipe
  if (pipe(pfd) == -1) {
	fprintf(stderr,"Pipe failed");

	/* close the open files */
	fclose(from);
	fclose(to);

	return 1;
  }

  /* now create the processes */
  pid = fork();

  if (pid < 0) {
	fprintf(stderr, "Fork Failed");

	/* close the files */
	fclose(from);
	fclose(to);

	return 1;
  }
  if (pid > 0) {  /* parent */
	/* close the end of the pipe we do not need */
	close(pfd[READ_END]);
        fclose(to);

	/* read from the input file and write to the pipe */
	while ((c = getc(from)) != EOF){
      			c = flip ? flipChar(c) : c;
			write(pfd[WRITE_END],&c, 1);
	}

	/* close the read end of the input file and the write end of the pipe */
	fclose(from);
	close(pfd[WRITE_END]);

	wait(NULL);
  }
  else{

         /* close the end of the pipe we do not need */
        close(pfd[WRITE_END]);

        /* read from the input file and write to the pipe */
	while ( (read(pfd[READ_END], &c, 1)) > 0) {
                        putc(c,to);
        }

        /* close the read end of the input file and the write end of the pipe */
        fclose(to);
        close(pfd[READ_END]);

  }

  return 0;
}
