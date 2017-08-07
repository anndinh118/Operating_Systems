/*************************************************************
* Names: Ann Dinh (masc2018) and Gracelina Narvaza (masc2017)
* Title: assignment1.h
* Course: CS 570 Operating Systems, Summer 2016
* Instructor: Guy Leonard
**************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define NUM_THREADS     7
sem_t FLAG;

void *PrintQuote(void *threadid);

/********************End of assignment1.h*********************/
