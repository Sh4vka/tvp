#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define YES 1
#define NO 0

int main() {
    int prep = open("prepare2", O_RDONLY);
    int vote = open("vote2", O_WRONLY);
    int decision = open("decision2", O_RDONLY);

    for (int i = 0; i < 3; i++) {

        int req;
        read(prep, &req, 1);

        printf("Node2: prepare received\n");

        int can_lock = 1; // имитация проверки A & B

        if (can_lock) {
            printf("Node2: VOTE YES\n");
            write(vote, &(int){YES}, 1);
        } else {
            printf("Node2: VOTE NO\n");
            write(vote, &(int){NO}, 1);
        }

        int d;
        read(decision, &d, 1);

        printf("Node2: decision = %d\n", d);

        if (d == NO) {
            printf("Node2: rollback\n");
        } else {
            printf("Node2: commit\n");
        }
    }

    return 0;
}
