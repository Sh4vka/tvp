#include <stdio.h>
#include <unistd.h>

int main() {
    printf("My:     %d\n", getpid());
    printf("Parent: %d\n", getppid());
    return 0;
}
