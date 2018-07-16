#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>			//Include Clock Time.

#include "utils.h"

int sh_count = 0;

int main(int argc, char **argv)
{
	struct tm *local;
	time_t start;
	time(&start);
	local = localtime(&start);
	printf("# Start time and date: %s", asctime(local));	//Outputs time at the beginning of execution.

	char *refStr, *tolStr;
	refStr = tolStr = NULL;
	int a, b, c, r, verbose = 0;

	for(a = 1; a < argc; a++)	//Code to handle command line parameters.
	{
		if(strcmp(argv[a], "-v") == 0)
			verbose  = 1;
		else if(strcmp(argv[a], "-r") == 0)
			refStr   = argv[a + 1];
		else if(strcmp(argv[a], "-t") == 0)
			tolStr   = argv[a+ 1];
	}

	if((argc != 5 && argc != 6) || refStr == NULL || tolStr == NULL)	//Check for valid input.
  	{
		fprintf(stderr, "Usage: %s -r ref -t tol\n", argv[0]);
		exit(1);
  	}

	float ref = strtof(refStr, NULL);       //Convert char refStr to float.
	float tol = strtof(tolStr, NULL);       //Convert char tolStr to float.

	int rct, cct;                           //Initializing rct and cct to take in size of matrix.
	scanf("%d", &rct);
	scanf("%d", &cct);

	float** sh_rows = (float **) malloc(rct*sizeof(float*));

	if(sh_rows == 0)
	{
		fprintf(stderr, "Can't allocate enough space.\n");
		exit(1);
	}

	for(b = 0; b < rct; b++)
	{
		float* row = (float *) malloc(cct * sizeof(float)); //Allocate required memory.
		if (row == 0)
		{
			fprintf(stderr, "Couldn't alocate sufficient row space.\n");
			exit(1);
		}
		sh_rows[b] = row;             	//Read data into sh_rows.
	}

	for(r = 0; r < rct; r++)           	//Two loops that gets each number and reads them into matrix.
		for(c = 0; c < cct; c++)
			scanf("%f", &sh_rows[r][c]);

	struct timeval tim;
	gettimeofday(&tim, NULL);				//Read and record clock.
	double t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);

	llist* plist = NULL;
	#pragma omp parallel for private(r, c)  //Initializing r, c to be private.
	for(r = 0; r < rct; r++)
		for(c = 0; c < cct; c++)
			if((approxEqual(sh_rows[r][c], ref, tol) == 1))         //Check if each position of the array is a match.
			{
				if(verbose)
					plist = add_list(plist, r, c, sh_rows[r][c], omp_get_thread_num());
				#pragma omp atomic
				sh_count++;
			}
    if(verbose)
	     print_list(plist);					//If verbose is true, list will be printed.

	 fprintf(stdout, "Found %d approximate matches.\n", sh_count);	//sh_count is output.

	gettimeofday(&tim, NULL);                                       //Code to correctly print out elapsed seconds.
	double t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
	fprintf(stdout, "Search time: %.6f(s)\n", t2 - t1);				//The search time is output.

	free(sh_rows);                  //Free everything created.
	plist = clear_list(plist);
	exit(0);
}
