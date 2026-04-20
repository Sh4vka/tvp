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

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 3);

    printf("Coordinator: waiting for nodes...\n");

    int clients[3];
    for (int i = 0; i < 3; i++) {
        clients[i] = accept(server_fd, NULL, NULL);
        printf("Node %d connected\n", i+1);
    }

    for (int t = 0; t < 3; t++) {
        printf("\nTransaction %d\n", t);

        int msg = PREPARE;

        for (int i = 0; i < 3; i++)
            write(clients[i], &msg, sizeof(int));

        int votes[3];
        for (int i = 0; i < 3; i++)
            read(clients[i], &votes[i], sizeof(int));

        int decision = YES;
        for (int i = 0; i < 3; i++)
            if (votes[i] != YES)
                decision = NO;

        msg = (decision == YES) ? COMMIT : ABORT;

        printf("Decision: %s\n", msg == COMMIT ? "COMMIT" : "ABORT");

        for (int i = 0; i < 3; i++)
            write(clients[i], &msg, sizeof(int));
    }

    close(server_fd);
    return 0;
}
