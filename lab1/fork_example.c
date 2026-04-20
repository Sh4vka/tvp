#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TEST 2
#define INFO 0

int main() {
    int val = 200;
    pid_t newp = fork();

#if TEST == 1
    printf("Parent: %d\n", getppid());
    printf("Child:  %d\n", getpid());
    val -= 100;
    printf("val : %d\n", val);
    return 0;

#elif TEST == 2
    switch (newp) {
        case 0:
            for (int i = 0; i < 3; i++) { printf("I am a CHILD %d\n", getpid());}
#if INFO == 1
            while(1);
#endif
            exit(1);

        case -1:
            break;

        default:
            for (int i = 0; i < 5; i++) { printf("I am a PARENT %d\n", getpid());}
#if INFO == 1
            while(1);
#endif
            exit(0);
    }
#endif
}
