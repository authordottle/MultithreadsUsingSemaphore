/********* consumer.c ***********/
#include "headers.h"

void *consumer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    item elem;
    
    // Read data
    while (1) {
        // buffer is empty
        while (ptr->in == ptr->out);
        // get an item from buffer
        elem = ptr->buffer[ptr->out];
        printf("%d\t%s", elem.id, elem.data);
        if (elem.id == -1) break;
        fprintf(fd, "%s", elem.data);
        ptr->out = (ptr->out + 1) % BUFFER_SIZE;
    }
}
