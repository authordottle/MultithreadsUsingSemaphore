#include "headers.h"

int main() {
    FILE *input  = fopen("input.txt", "r");    // input fd
    FILE *output = fopen("output.txt", "w");   // output fd
    if (input == NULL || output == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }

    // initialize bounded buffer
    shared_struct *ptr = (shared_struct *) malloc(sizeof(shared_struct));
    ptr->in = 0;
    ptr->out = 0;

    // initialize semaphores
    if (sem_init(&mutex, 0, 1) == -1) {
        printf("Initilize semaphore failed\n");
        exit(-1);
    }
    if (sem_init(&empty, 0, BUFFER_SIZE) == -1) {
        printf("Initilize semaphore failed\n");
        exit(-1);
    }
    if (sem_init(&full, 0, 0) == -1) {
        printf("Initilize semaphore failed\n");
        exit(-1);
    }

    // initialize pthread args
    pthread_args *p_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_args->ptr = ptr;
    p_args->fd = input;

    pthread_args *c_args = (pthread_args *) malloc(sizeof(pthread_args));
    c_args->ptr = ptr;
    c_args->fd = output;

    pthread_attr_t attr;            // set of thread attributes
    pthread_attr_init(&attr);       // get the default attributes

    pthread_t producer_t[PRODUCER_NUM]; // producer thread identifiers
    pthread_t consumer_t[CONSUMER_NUM]; // consumer thread identifiers

    // create producer and consumer threads
    for (int i = 0; i < PRODUCER_NUM; i++) {
        pthread_create(&(producer_t[i]), &attr, producer, p_args);
    } 
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_create(&(consumer_t[i]), &attr, consumer, c_args);
    }

    // wait for working threads
    for (int i = 0; i < PRODUCER_NUM; i++) {
        pthread_join(consumer_t[i], NULL);
    }
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_join(producer_t[i], NULL);
    }

    // destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    // close open files
    fclose(input);
    fclose(output);

    // free
    free(p_args);
    free(c_args);
    free(ptr);

    exit(0);
}
