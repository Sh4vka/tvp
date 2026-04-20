#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

typedef struct {
    int A;
    int B;
    int C;
} shared_t;

void node1(shared_t *s) {
    printf("Node1: Lock A\n");
    s->A = 0;
    sleep(1);
    printf("Node1: waiting for B\n");
    while (s->B == 0);
    s->B = 0;
    printf("Node1: working\n");
    s->B = 1; s->A = 1;
    exit(0);
}

void node2(shared_t *s) {
    printf("Node2: Lock A\n");
    s->B = 0;
    sleep(1);
    printf("Node2: waiting for B\n");
    while (s->C == 0);
    s->C = 0;
    printf("Node2: working\n");
    s->C = 1; s->B = 1;
    exit(0);
}

void node3(shared_t *s) {
    printf("Node3: Lock A\n");
    s->C = 0;
    sleep(1);
    printf("Node3: waiting for B\n");
    while (s->A == 0);
    s->A = 0;
    printf("Node3: working\n");
    s->C = 1; s->A = 1;
    exit(0);
}

int main() {
    shared_t *s = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    s->A = 1;
    s->B = 1;
    s->C = 1;
    if (fork() == 0) node1(s);
    if (fork() == 0) node2(s);
    if (fork() == 0) node3(s);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
