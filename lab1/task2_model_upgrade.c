#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct {
    int A;
    int B;
    int C;
    sem_t mutex_A;
    sem_t mutex_B;
    sem_t mutex_C;
} shared_t;

void node1(shared_t *s) {
    sem_wait(&s->mutex_A);
    printf("Node1: locked A\n");
    s->A = 0;
    
    printf("Node1: trying to lock B\n");
    sem_wait(&s->mutex_B);
    s->B = 0;
    
    printf("Node1: working with A and B\n");
    sleep(1);
    
    s->A = 1;
    s->B = 1;
    sem_post(&s->mutex_B);
    sem_post(&s->mutex_A);
    
    printf("Node1: finished\n");
    exit(0);
}

void node2(shared_t *s) {
    sem_wait(&s->mutex_B);
    printf("Node2: locked B\n");
    s->B = 0;
    
    printf("Node2: trying to lock C\n");
    sem_wait(&s->mutex_C);
    s->C = 0;
    
    printf("Node2: working with B and C\n");
    sleep(1);
    
    s->B = 1;
    s->C = 1;
    sem_post(&s->mutex_C);
    sem_post(&s->mutex_B);
    
    printf("Node2: finished\n");
    exit(0);
}

void node3(shared_t *s) {
    sem_wait(&s->mutex_A);
    printf("Node3: locked A\n");
    s->A = 0;
    
    printf("Node3: trying to lock C\n");
    sem_wait(&s->mutex_C);
    s->C = 0;
    
    printf("Node3: working with A and C\n");
    sleep(1);
    
    s->A = 1;
    s->C = 1;
    sem_post(&s->mutex_C);
    sem_post(&s->mutex_A);
    
    printf("Node3: finished\n");
    exit(0);
}

int main() {
    shared_t *s = mmap(NULL, sizeof(shared_t), 
                       PROT_READ | PROT_WRITE, 
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    s->A = 1;
    s->B = 1;
    s->C = 1;
    
    sem_init(&s->mutex_A, 1, 1);
    sem_init(&s->mutex_B, 1, 1);
    sem_init(&s->mutex_C, 1, 1);
    
    if (fork() == 0) node1(s);
    if (fork() == 0) node2(s);
    if (fork() == 0) node3(s);
    
    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    sem_destroy(&s->mutex_A);
    sem_destroy(&s->mutex_B);
    sem_destroy(&s->mutex_C);
    munmap(s, sizeof(shared_t));
    
    return 0;
}
