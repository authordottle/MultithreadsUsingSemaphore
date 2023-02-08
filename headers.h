/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define PRODUCER_NUM 3
#define CONSUMER_NUM 3
#define SHM_NAME "OS"

typedef struct {
    int id;                         // item id
    char data[100];                 // item data
} item;

typedef struct {
    int in;                         // shared variable in
    int out;                        // shared variable out
    item buffer[BUFFER_SIZE];       // bounded buffer with size BUFFER_SIZE
} shared_struct;

sem_t mutex;                        // protect buffer
sem_t full;                        // 
sem_t empty;                        //

typedef struct {
    shared_struct *ptr;             // shared by producer threads and consumer threads
    FILE *fd;                       // file descriptor of input file or output file
} pthread_args;

void *producer(void *args);         // producer thread
void *consumer(void *args);         // consumer thread
