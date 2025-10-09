#include <pthread.h>    // for threads
#include <stdio.h>      // for printf()
#include <stdint.h>     // for uint_t
#include <stdlib.h>     // for rand(), srand()
#include <unistd.h>     // for sleep()
#include <time.h>       // for time()

void *sorter(void* arg){
    uint64_t *value = (uint64_t *)arg;
    sleep(*value);
    printf("%lu\n", *value);
    return NULL;
}

int main(){
    uint8_t num_threads = 12;

    srand(time(NULL));

    pthread_t thread[num_threads];

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

    printf("Sorting values:\n");
    for(uint8_t i=0; i<num_threads; i++){
        pthread_create(&thread[i], NULL, sorter, (void *)&value[i]);
    }

    for(uint8_t i=0; i<num_threads; i++){
        pthread_join(thread[i], NULL);
    }

    printf("\nAll values sorted!\n");

    return 0;
}
