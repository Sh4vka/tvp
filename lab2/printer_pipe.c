#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define TOKEN 1

void task(const char* name, int rfd, int wfd) {
    for (int i = 0; i < 3; i++) {
        printf("%s: waiting for printer\n", name);
        char token;
        read(rfd, &token, 1);
        printf("%s: printer is free, taking\n", name);
        printf("%s: printing...\n", name);
        sleep(1);
        printf("%s: releasing printer\n", name);
        write(wfd, &token, 1);
    }
    exit(0);
}

int main() {
    int p[2];
    if (pipe(p) == -1) {
        perror("pipe");
        exit(1);
    }
    char token = TOKEN;
    write(p[1], &token, 1);
    pid_t pid1 = fork();
    if(pid1 == 0) {
        close(p[1]);
        task("Task1", p[0], p[1]);
    }
    pid_t pid2 = fork();
    if(pid2 == 0) {
        close(p[1]);
        task("Task2", p[0], p[1]);
    }
    close(p[0]);
    close(p[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}
