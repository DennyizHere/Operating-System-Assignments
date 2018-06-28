#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void semInit(void);
void * heart(void *);
void * spade(void *);
void * diamond(void *);
void * club(void *);
