#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAXLINE 1000
char *readline(FILE *rfile) {
  /* Read a line from a file and return a new line string object */
  char buf[MAXLINE];
  int len;
  char *result = NULL;
  char *line = fgets(buf, MAXLINE, rfile);
  if (line) {
    len = strnlen(buf, MAXLINE);
    result = strncpy(malloc(len+1),buf,len+1);
  }
  return result;
}

typedef struct sharedobject {
  FILE *rfile;
  pthread_mutex_t solock;
  int flag;
  int linenum;
  char *line;
} so_t;

typedef struct targ {
  long tid;
  so_t *soptr;
} targ_t;

int waittill(so_t *so, int val) {
  /* spin as long as flag retains val */
  int flag;
   while (1) {
    int lw = pthread_mutex_lock(&so->solock); /* gain access */
    //printf(":%d: ", lw);
    flag = so->flag;
    if (flag == val) return 1; /* return with the object locked */
    pthread_mutex_unlock(&so->solock); /* free it for others */
   }
}

int release(so_t *so) {
  return pthread_mutex_unlock(&so->solock);
}

void *producer(void *arg) {
  so_t *so = arg;
  int *ret = malloc(sizeof(int));
  FILE *rfile = so->rfile;
  int i;
  int w = 0;
  char *line;
  for (i = 0; (line = readline(rfile)); i++) {
    waittill(so, 0);		/* grab lock when empty */
    so->linenum = i;		/* update the shared state */
    so->line = line;		/* share the line */
    so->flag = 1;		/* mark full */
    release(so);		/* release the loc */
    fprintf(stdout, "Prod: [%d] %s", i, line);
  }
  waittill(so, 0);		/* grab lock when empty */
  so->line = NULL;
  so->flag = 1;
  printf("Prod: %d lines\n", i);
  release(so);		/* release the loc */
  *ret = i;
  pthread_exit(ret);
}

void *consumer(void *arg) {
  targ_t *targ = (targ_t *) arg;
  long tid = targ->tid;
  so_t *so = targ->soptr;
  int *ret = malloc(sizeof(int));
  int i = 0;;
  int len;
  char *line;
  int w = 0;
  printf("Con %ld starting\n",tid);
  while (waittill(so, 1) && 
	 (line = so->line)) {
    len = strlen(line);
    printf("Cons %ld: [%d:%d] %s", tid, i, so->linenum, line);
    so->flag = 0;
    release(so);		/* release the loc */
    i++;
  }
  printf("Cons %ld: %d lines\n", tid, i);
  release(so);		/* release the loc */
  *ret = i;
  pthread_exit(ret);
}

#define CONSUMERS 4

int main (int argc, char *argv[])
{
  pthread_t prod;
  pthread_t cons[CONSUMERS];
  targ_t carg[CONSUMERS];
  int rc;
  long t;
  int *ret;
  FILE *rfile;
  int i;
  so_t *share = malloc(sizeof(so_t));
  
  if (argc < 2) {
    fprintf(stderr,"Usage %s filename",argv[0]);
    exit(0);
  }

  rfile = fopen((char *) argv[1], "r");
  if (!rfile) {
    printf("error opening %s\n",argv[0]);
    exit(0);
  } 
  
  share->rfile = rfile;
  share->line = NULL;
  share->flag = 0;		/* initially empty */
  pthread_mutex_init(&share->solock, NULL);

  pthread_create(&prod, NULL, producer, share);

  for (i=0; i<CONSUMERS; i++) {
    carg[i].tid = i;
    carg[i].soptr = share;
    pthread_create(&cons[i], NULL, consumer, &carg[i]);
  }

  printf("main continuing\n");

  rc = pthread_join(prod, (void **) &ret);
  printf("main: producer joined with %d\n", *ret);
  for (i=0; i<CONSUMERS; i++) {
    rc = pthread_join(cons[i], (void **) &ret);
    printf("main: consumer %d joined with %d\n", i, *ret);
  }
  share->flag = 0;
  pthread_mutex_destroy(&share->solock);
  pthread_exit(NULL);
  exit(0);
}
