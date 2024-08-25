#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5 // Number of philosophers
#define EATING_LIMIT 3 // Number of times each philosopher eats

pthread_mutex_t forks[N];
pthread_t philosophers[N];
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
int eat_count[N] = {0};

void *philosopher(void *num) {
    int id = *(int *)num;
    while (1) {
        pthread_mutex_lock(&count_mutex);
        if (eat_count[id] >= EATING_LIMIT) {
            pthread_mutex_unlock(&count_mutex);
            break;
        }
        pthread_mutex_unlock(&count_mutex);

        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        // Pick up forks
        pthread_mutex_lock(&forks[id]);
        pthread_mutex_lock(&forks[(id + 1) % N]);

        printf("Philosopher %d is eating.\n", id);
        sleep(1);

        pthread_mutex_lock(&count_mutex);
        eat_count[id]++;
        pthread_mutex_unlock(&count_mutex);

        // Put down forks
        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % N]);
    }
    printf("Philosopher %d is done eating.\n", id);
    return NULL;
}

int main() {
    int i;
    int ids[N];

    for (i = 0; i < N; i++)
        pthread_mutex_init(&forks[i], NULL);

    for (i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    for (i = 0; i < N; i++)
        pthread_mutex_destroy(&forks[i]);

    pthread_mutex_destroy(&count_mutex);

    return 0;
}