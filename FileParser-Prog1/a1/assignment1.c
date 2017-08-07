/*************************************************************
* Names: Ann Dinh (masc2018) and Gracelina Narvaza (masc2017)
* Title: assignment1.c
* Course: CS 570 Operating Systems, Summer 2016
* Instructor: Guy Leonard
**************************************************************/

#include "assignment1.h"

void *PrintQuote(void *threadid)
{
    long tid;
    tid = (long)threadid;
    for (tid = 1; tid <= 7; tid++){
      sem_wait(&FLAG);
      FILE *fp = fopen("QUOTE.txt", "a");
      if (tid%2 == 0) {
        fprintf(fp, "%ld %s", tid, "\"Controlling complexity is the essence of computer programming.\"\n\r");
        printf("Thread %ld is running\n", tid);
      } //print if thread is even
      else {
        fprintf(fp, "%ld %s", tid, "\"Computer science is no more about computer than astronomy is about telescopes.\"\n\r");
        printf("Thread %ld is running\n", tid);
      } //print if thread is odd
      fclose(fp); //close "QUOTE.txt"
      sem_post(&FLAG); //release/unlocks the semaphore
    }
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pid_t pid = getpid();
    FILE *fp = fopen("QUOTE.txt", "w");
    fprintf(fp, "%d\r\n", pid); //write process ID, Carriage Return, Newline
    fclose(fp);

    sem_init(&FLAG, 0, 7);

    pthread_t threads[NUM_THREADS];
    int createdThread;
    long t;
    for(t=0; t<NUM_THREADS; t++){
       createdThread = pthread_create(&threads[t], NULL, PrintQuote, NULL);
       if (createdThread) { //if pthread_create returns a non-successful value
          printf("ERROR; return code from pthread_create() is %d\n", createdThread);
          exit(-1);
        }
    }
    int i;
    for (i=0; i<NUM_THREADS; i++) {
      pthread_join(threads[i],NULL);
    } //blocks/waits for all seven threads to complete work

    sem_destroy(&FLAG);
    printf("Success!\n");
    pthread_exit(NULL);
}

/********************End of assignment1.c*********************/
