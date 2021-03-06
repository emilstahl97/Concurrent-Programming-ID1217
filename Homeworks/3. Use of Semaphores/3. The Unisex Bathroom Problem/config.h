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
#define MAX_VISITS 20     /* Max number of times the bathroom is used. */
#define MAX_TIMEIN 3     /* Max seconds in the bathroom. */
#define MAX_INTERVAL 5   /* Max time between visits to the bathroom. */

#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define CYN "\x1B[36m"
#define BLU  "\x1B[34m"
#define LBLU "\x1b[94m"
#define RED  "\x1B[31m"
#define MAG "\x1b[35m"
#define LMAG "\x1b[95m"
#define RESET "\x1B[0m"
