#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCERS 2
#define CONSUMERS 2
#define ITEM_LIMIT 20 // Number of items to produce/consume before stopping

int buffer[BUFFER_SIZE];
int count = 0;
int items_produced = 0;
int items_consumed = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_produce;
pthread_cond_t cond_consume;

void *producer(void *num) {
    int id = *(int *)num;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (items_produced >= ITEM_LIMIT) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        while (count == BUFFER_SIZE)
            pthread_cond_wait(&cond_produce, &mutex);

        buffer[count++] = rand() % 100;
        items_produced++;
        printf("Producer %d produced item %d (Total produced: %d)\n", id, buffer[count - 1], items_produced);

        pthread_cond_signal(&cond_consume);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *num) {
    int id = *(int *)num;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (items_consumed >= ITEM_LIMIT) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        while (count == 0)
            pthread_cond_wait(&cond_consume, &mutex);

        printf("Consumer %d consumed item %d (Total consumed: %d)\n", id, buffer[--count], ++items_consumed);

        pthread_cond_signal(&cond_produce);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int ids[PRODUCERS + CONSUMERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_produce, NULL);
    pthread_cond_init(&cond_consume, NULL);

    for (int i = 0; i < PRODUCERS; i++) {
        ids[i] = i;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        ids[PRODUCERS + i] = i;
        pthread_create(&cons[i], NULL, consumer, &ids[PRODUCERS + i]);
    }

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_produce);
    pthread_cond_destroy(&cond_consume);

    return 0;
}