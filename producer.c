/******* producer.c *******/
#include "headers.h"

void *producer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    item elem;
    
    int id  = 0;
    int end = 0;

    while (1) {
        elem.id = id++;
        // Read each line from the input file
        if (fgets(elem.data, 100, fd) == NULL) {
            elem.id = -1;
            elem.data[0] = 0;
            end = 1;
        }
        // buffer is full
        while (((ptr->in + 1) % BUFFER_SIZE) == ptr->out);
        // put an item in buffer
        ptr->buffer[ptr->in] = elem;
        ptr->in = (ptr->in + 1) % BUFFER_SIZE;
        // if there is no more data to be read from the file
        if (end) break;
    }
}
