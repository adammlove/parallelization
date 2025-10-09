#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

void* hello(void* arg){
    printf("Hello from thread %lu\n", (uint64_t)pthread_self());
    return NULL;
}

int main(){
    pthread_t thread[4];
    for(uint8_t i=0; i<4; i++){
        pthread_create(&thread[i], NULL, hello, NULL);
    }

    for(uint8_t i=0; i<4; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}
