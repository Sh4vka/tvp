#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define YES 1
#define NO 0
#define N 3

int main() {
    int prep[3], vote[3], decision[3];
    
    prep[0] = open("prepare1", O_WRONLY);
    prep[1] = open("prepare2", O_WRONLY);
    prep[2] = open("prepare3", O_WRONLY);

    vote[0] = open("vote1", O_RDONLY);
    vote[1] = open("vote2", O_RDONLY);
    vote[2] = open("vote3", O_RDONLY);

    decision[0] = open("decision1", O_WRONLY);
    decision[1] = open("decision2", O_WRONLY);
    decision[2] = open("decision3", O_WRONLY);

    for (int i = 0; i < N; i++) {
        printf("Coordinator: transaction %d\n", i);
        int req = 1;

        write(prep[0], &req, 1);
        write(prep[1], &req, 1);
        write(prep[2], &req, 1);

        int v1, v2, v3;

        read(vote[0], &v1, 1)
        read(vote[1], &v2, 1)
        read(vote[2], &v3, 1)

        int result = (v1 == YES && v2 == YES && v3 == YES) ? YES : NO;

        printf("Coordinator: decision = %d\n", result);

        write(decision[0], &result, 1);
        write(decision[1], &result, 1);
        write(decision[2], &result, 1);
    }
    return 0;
}
