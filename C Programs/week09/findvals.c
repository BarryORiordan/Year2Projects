#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>           //Include Clock Time.

#include "utils.h"

#define NUM_THREADS 2

typedef struct sharedobject
{
    int sh_rct;                 //Shared row count.
    int sh_cct;                 //Shared col count.
    int verbose;                //Shared flag.
    float sh_ref;               //Shared.
    float sh_tol;
    float** d_matrix;           //Shared data.
} so_t;

so_t* shared_data;              //Threads can read the thread global data from this variable.

pthread_mutex_t mutexlock;      //Mutex lock variable.

int sh_count = 0;               //Global variable that counts approximate matches.

void *runner(void *param);      //Thread prototype.

int main(int argc, char **argv)
{
    char *refStr, *tolStr;
    refStr = tolStr = NULL;
    int a, b, r, c, verbose = 0;

    for(a = 1; a < argc; a++)   //Code to handle command line parameters.
    {
        if(strcmp(argv[a], "-v") == 0)
            verbose = 1;
        else if(strcmp(argv[a], "-r") == 0)
            refStr = argv[a + 1];
        else if(strcmp(argv[a], "-t") == 0)
            tolStr = argv[a + 1];
    }

    if((argc != 5 && argc != 6) || refStr == NULL || tolStr == NULL)    //Check for valid input.
    {
        fprintf(stderr, "Usage: %s -r ref -t tol\n", argv[0]);
        exit(1);
    }

    struct tm *local;
    time_t start;
    time(&start);
    local = localtime(&start);
    printf("# Start time and date: %s", asctime(local));    //Outputs time at the beginning of execution.

    struct timeval tim;
    gettimeofday(&tim, NULL);               //Read and record clock.
    double t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);

    float ref = strtof(refStr, NULL);       //Convert char refStr to float.
    float tol = strtof(tolStr, NULL);       //Convert char tolStr to float.

    int rct, cct;                           //Initializing rct and cct to take in size of matrix.
    scanf("%d", &rct);
    scanf("%d", &cct);

    shared_data = malloc(sizeof(so_t));     //Allocated memory for shared data.

    float** sh_rows = (float **) malloc(rct *sizeof(float*));
     if(sh_rows == 0)
    {
        fprintf(stderr, "Can't allocate enough space.\n");
        exit(1);
    }

    for(b = 0; b < rct; b++)
    {
        float* row = (float *) malloc(cct * sizeof(float)); //Allocate required memory.
        if(row == 0)
        {
            fprintf(stderr, "Couldn't alocate sufficient row space.\n");
            exit(1);
        }
        sh_rows[b] = row;                //Read data into sh_rows.
    }

    for(r = 0; r < rct; r++)            //Two loops that gets each number and reads them into matrix.
        for(c = 0; c < cct; c++)
            scanf("%f", &sh_rows[r][c]);

    shared_data->sh_rct   = rct;        //Load all the input data into shared_data.
    shared_data->sh_cct   = cct;
    shared_data->d_matrix = sh_rows;
    shared_data->sh_ref   = ref;
    shared_data->sh_tol   = tol;
    shared_data->verbose  = verbose;    //Multi-threading begins below.

    pthread_t tid[NUM_THREADS];         //Array to hold thread IDs.
    int t_index[NUM_THREADS] = {0, 1};  //Array to pass an index to each thread indicating the thread number.

    llist* plist = NULL;		        //To hold linked list to print out if "-v" option is used.

    pthread_mutex_init(&mutexlock, NULL);   //Initlialise lock.

    int ti;
    for(ti = 0; ti < NUM_THREADS; ti++)                         //Creating threads.
        pthread_create(&tid[ti], NULL, runner, &t_index[ti]);   //Passes on correct thread index.

    for(ti = 0; ti < NUM_THREADS; ti++)
    {
        pthread_join(tid[ti], (void **) &plist);
        if(verbose)
            print_list(plist);      //List is printed.
    }

    fprintf(stdout, "Found %d approximate matches.\n", sh_count);   //sh_count is output.

    gettimeofday(&tim, NULL);                                       //Code to correctly print out elapsed seconds.
    double t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
    fprintf(stdout, "Elapsed time: %.6f(s)\n", t2 - t1);

    int pthread_mutex_destroy(pthread_mutex_t *mutexlock);          //Destroy lock.

    free(sh_rows);                  //Free everything created.
    free(shared_data);
    plist = (clear_list(plist));
    exit(0);
}

void *runner(void *param)
{
    int tid = *((int *) param);

    int s_row = 0, e_row = 0;
    if(tid == 0)                            //e_rows will be scanned.
        e_row = shared_data->sh_rct / 2;
    else                                    //Both rows will be scanned.
    {
        e_row = shared_data->sh_rct;
        s_row = shared_data->sh_rct / 2;
    }

    llist* print_list = NULL;
    int r = 0, c = 0, equals = 0;
    float tol = shared_data->sh_tol;
    float ref = shared_data->sh_ref;
    int verbose = shared_data->verbose;
    float** sh_rows = shared_data->d_matrix;

    for(r = s_row; r < e_row; r++)
        for(c = 0; c < shared_data->sh_cct; c++)
        {
            equals = (approxEqual(sh_rows[r][c], ref, tol) == 1);       // Check if sh_rows{r][c] matches the given test.
            if(equals)      //Code to increment count in a "thread safe" manner.
            {
                pthread_mutex_lock(&mutexlock);
                sh_count++;
                pthread_mutex_unlock(&mutexlock);

                if(verbose)
                    print_list = add_list(print_list, r, c, sh_rows[r][c]); //Add match to printed list.
            }
        }
    pthread_exit((void*)print_list);        //Return list back to calling thread.
}
