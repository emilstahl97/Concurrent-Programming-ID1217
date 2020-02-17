#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define SHARED 0        /* The semaphores should not be shared. */
#define MAX_MEN 10     /* The max number of males possible. */
#define MAX_WOMEN 10     /* The max number of women possible. */
#define MAXTIMES 20     /* Max number of times the bathroom is used. */
#define MAXTIMEIN 3     /* Max seconds in the bathroom. */
#define MINTIMEIN 1     /* Min seconds in the bathroom. */
#define MAXINTERVAL 5   /* Max time between visits to the bathroom. */
#define MININTERVAL 1   /* Min time between visits to the bathroom. */

#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"
