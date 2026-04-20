#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define YES 1
#define NO 0

int main() {
    int prep = open("prepare3", O_RDONLY);
    int vote = open("vote3", O_WRONLY);
    int decision = open("decision3", O_RDONLY);

    for (int i = 0; i < 3; i++) {

        int req;
        read(prep, &req, 1);

        printf("Node3: prepare received\n");

        int can_lock = 1; // имитация проверки A & B

        if (can_lock) {
            printf("Node3: VOTE YES\n");
            write(vote, &(int){YES}, 1);
        } else {
            printf("Node3: VOTE NO\n");
            write(vote, &(int){NO}, 1);
        }

        int d;
        read(decision, &d, 1);

        printf("Node3: decision = %d\n", d);

        if (d == NO) {
            printf("Node3: rollback\n");
        } else {
            printf("Node3: commit\n");
        }
    }

    return 0;
}
