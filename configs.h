#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <pthread.h>
#include <sys/resource.h>
#include <errno.h>
#include <ulimit.h>
#include"dll_loader.h"

#define MAX_CONNECTIONS 100 /*Total connectios held by server*/
#define QUEUE_SIZE 50 /*Limits the size of queue used by dispatcher and server threads*/
#define THREAD_POOL_SIZE 8  /*To limit the number of threads both server as well as threads used by the dispatcher*/
#define OPEN_FILE_FD 10 /*Total file descriptors to be opened*/
#define MEMORY_LIMIT_IN_MB 10 /*Setting up the memory limit*/
#define SET_TOTAL_FD_LIMIT 0 /*Change it to 1 to allow once OPEN_FILE_FD limit is reached the server will reset the connection*/
#define SET_DISPATCHER 1 /*Change it to 0 to disable dispatcher*/
