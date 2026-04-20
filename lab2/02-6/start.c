#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

void start_proc(const char *path, const char *arg) {
    pid_t pid = fork();

    if (pid == 0) {
        execl(path, path, arg, NULL);
        perror("execl failed");
        exit(1);
    }
}

int main() {
    start_proc("./node", "1");
    start_proc("./node", "2");
    start_proc("./node", "3");

    while (wait(NULL) > 0);

    return 0;
}
