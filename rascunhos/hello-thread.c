#include <pthread.h>
#include <stdio.h>
// #include <stdlib.h>

#define NUMBER_OF_THREADS 10

void *print_hello_world(void *tid) {
    printf("\nHello World. Greeting from thread %d0", tid);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i;

    for( i=0; i < NUMBER_OF_THREADS; i++) {
        printf("\nMain here. Creating thread %d0", i);
        status = pthread_create(&threads[i], NULL, print_hello_world, (void *)i);

        if (status != 0) {
            printf("Ooops, pthread returned error code %d0, status");
            exit(-1);
        }
    }
    
    exit(NULL);

}