#include <pthread.h>    // for threads
#include <stdio.h>      // for printf()
#include <stdint.h>     // for uint_t
#include <stdlib.h>     // for rand(), srand()
#include <unistd.h>     // for sleep()
#include <time.h>       // for time()

// sleep for the time specified by the input value
// then print the input value
void *sorter(void* arg){
    uint64_t *value = (uint64_t *)arg;
    sleep(*value);
    printf("%lu\n", *value);
    return NULL;
}

int main(){
    // set the number of threads
    uint8_t num_threads = 12;

    // seed the random number generator
    srand(time(NULL));

    // create and print unsorted values
    uint64_t value[num_threads];
    printf("Unsorted array: [");
    for(uint8_t i=0; i<num_threads; i++){
        value[i] = rand()%20;
        printf("%lu", value[i]);

        if(i != num_threads-1){
            printf(", ");
        }
    }
    printf("]\n\n");

    // create threads and sort values
    pthread_t thread[num_threads];
    printf("Sorting values:\n");
    for(uint8_t i=0; i<num_threads; i++){
        pthread_create(&thread[i], NULL, sorter, (void *)&value[i]);
    }

    // wait for all threads to finish
    for(uint8_t i=0; i<num_threads; i++){
        pthread_join(thread[i], NULL);
    }
    printf("\nAll values sorted!\n");

    return 0;
}
