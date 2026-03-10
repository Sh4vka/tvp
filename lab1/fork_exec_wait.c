#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t newp = fork();
    switch (newp) {
        case 0:
            execlp("ls", "ls", "-l", NULL);
            exit(1);

        case -1:
            break;

        default:
            wait(NULL);
            printf("Child process finished\n");
            exit(0);
    }
    return 0;
}
