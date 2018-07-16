#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char **argv)
{
    float ref, tol;
    char *refStr, *tolStr;
    refStr = tolStr = NULL;

    if(strcmp(argv[1], "-r") == 0)                                  //Check to see if "-r" is in argv[1].
    {
        refStr = argv[2];                                           //Input for refStr therefore in argv[2].
        tolStr = argv[4];                                           //Input for tolStr therefore in argv[4].
    }
    else                                                            //Otherwise "-r" is in argv[3].
    {
        tolStr = argv[2];                                           //Then just swap the storage variables.
        refStr = argv[4];
    }

    ref = strtof(refStr, NULL);                                     //Convert char refStr to float.
    tol = strtof(tolStr, NULL);                                     //Convert char tolStr to float.

    if(argc != 5 || refStr == NULL || tolStr == NULL)               //Check if arguments were passed in correctly.
    {
        fprintf(stderr, "Usage: %s -r ref -t tol\n", argv[0]);
        exit(1);
    }

    struct tm *local;
    time_t start, end;

    time(&start);               									//Read and record clock.
    local = localtime(&start);
    printf("# Start time and date: %s", asctime(local));			//Outputs time at the beginning of execution.

    int rct, cct;         											//Initializing rct and cct to take in size of matrix.
    scanf("%d", &rct);
    scanf("%d", &cct);

    float** rows = (float **) malloc(rct * sizeof(float *));        //Allocate memory to store (rct x cct) matrix of floats.
    if(rows == 0)
    {
        fprintf(stderr, "Couldn't allocate sufficient space.\n");   //If rows = 0, space couldn't be allocated.
        exit(1);
    }

    int i;
    for(i = 0; i < rct; i++)                                        //Loop to allocate memory to store row space.
    {
        float* row = (float *) malloc(cct * sizeof(float));
        if(row == 0)
        {
            fprintf(stderr, "Couldn't allocate sufficient row space.\n");
            exit(1);
        }
        rows[i] = row;
    }

    int r, c;
    for(r = 0; r < rct; r++)                                        //Loop to read matrix into array rows.
        for(c = 0; c < cct; c++)
            scanf("%f", &rows[r][c]);

    int result, count = 0;
    for(r = 0; r < rct; r++)        			                    //Loop to go through matrix, checking if each value is within the tolerance.
        for(c = 0; c < cct; c++)
        {
            result = approxEqual(rows[r][c], ref, tol);
            if(result == 1)
            {
                fprintf(stdout, "r=%d, c=%d: %.6f\n", r, c, rows[r][c]);
                count++;
            }
        }

    fprintf(stdout, "Found %d approximate matches.\n", count);

    time(&end);													    //Read and record clock.
    local = localtime(&end);
    printf("# End time and date: %s", asctime(local));			    //Outputs time at the end of execution.

    return 0;
}
