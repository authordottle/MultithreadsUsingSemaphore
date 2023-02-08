/********* consumer.c ***********/
#include "headers.h"

void *consumer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    item elem;
    
    static int end = 0;
    // Read data
    while (1) {
        sem_wait(&full);
        // acquire lock
        sem_wait(&mutex);

        if (!end) {
            // get an item from buffer
            elem = ptr->buffer[ptr->out];
            //printf("%d\t%s", elem.id, elem.data);
            if (elem.id >= 0) {
                fprintf(fd, "%s", elem.data);
                ptr->out = (ptr->out + 1) % BUFFER_SIZE;
            } else {
                end = 1;
            }
        }

        // release lock
        sem_post(&mutex);
        // notify producers
        sem_post(&empty);
        if (end) {
            // notify all other consumers
            //sem_post(&sem_c);
            break;
        }
    }
}
