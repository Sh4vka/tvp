#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define YES 2
#define NO  3
#define PREPARE 1
#define COMMIT 4
#define ABORT  5

int main(int argc, char *argv[]) {
    int id = atoi(argv[1]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    for (int i = 0; i < 3; i++) {
        int msg;

        read(sock, &msg, sizeof(int));

        if (msg == PREPARE) {
            printf("Node%d: PREPARE\n", id);

            int vote = YES; 

            printf("Node%d: vote = %s\n", id,
                   vote == YES ? "YES" : "NO");

            write(sock, &vote, sizeof(int));
        }

        read(sock, &msg, sizeof(int));

        if (msg == COMMIT) {
            printf("Node%d: COMMIT\n", id);
        } else {
            printf("Node%d: ABORT\n", id);
        }
    }

    close(sock);
    return 0;
}
