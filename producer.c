/******* producer.c *******/
#include "headers.h"

void *producer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    item elem;
    
    // shared by all threads
    static int id  = 0;
    static int end = 0;

    while (1) {
        sem_wait(&empty);
        // acquire lock
        sem_wait(&mutex);

        if (!end) {
            elem.id = id++;
            // Read each line from the input file
            if (fgets(elem.data, 100, fd) == NULL) {
                elem.id = -1;
                elem.data[0] = 0;
                end = 1;
            }
            // put an item in buffer
            ptr->buffer[ptr->in] = elem;
            ptr->in = (ptr->in + 1) % BUFFER_SIZE;
        }

        // release lock
        sem_post(&mutex);
        // notify consumers
        sem_post(&full);
        // if there is no more data to be read from the file
        if (end) {
            break;
        }
    }
}
