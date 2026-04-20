#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *printer_busy = (int*) shmat(shmid, NULL, 0);

    *printer_busy = 0;

    pid_t p1 = fork();
    if (p1 == 0) {
        printf("Task1: checking printer\n");
        if (*printer_busy == 0) {
            printf("Task1: printer is free\n");
            usleep(1000);
            *printer_busy = 1;
            printf("Task1: printing...\n");
            usleep(10000);
            printf("Task1: releasing\n");
            *printer_busy = 0;
        }
        exit(0);
    }
    pid_t p2 = fork();
    if (p2 == 0) {
        printf("Task2: checking printer\n");
        if (*printer_busy == 0) {
            printf("Task2: printer is free\n");
            usleep(1000);
            *printer_busy = 1;
            printf("Task2: printing...\n");
            usleep(10000);
            printf("Task2: releasing\n");
            *printer_busy = 0;
        }
        exit(0);
    }
    wait(NULL);
    wait(NULL);

    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

