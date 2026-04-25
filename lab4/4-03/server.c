#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define REQUEST_TYPE 1

struct request {
    long mtype;
    pid_t pid;
    char text[256];
};

struct response {
    long mtype;
    char text[256];
};

int msqid;

void cleanup(int sig) {
    printf("\nShutting down server...\n");
    msgctl(msqid, IPC_RMID, NULL);
    exit(0);
}

int main() {
    key_t key = ftok("server.c", 'A');
    msqid = msgget(key, 0666 | IPC_CREAT);

    signal(SIGINT, cleanup);

    struct request req;
    struct response resp;

    printf("Server started...\n");

    while (1) {
        if (msgrcv(msqid, &req, sizeof(req) - sizeof(long),
                   REQUEST_TYPE, 0) < 0) {
            perror("msgrcv");
            continue;
        }

        printf("Request from PID=%d: %s\n", req.pid, req.text);

        time_t now = time(NULL);
        snprintf(resp.text, sizeof(resp.text),
                 "Time: %s", ctime(&now));

        resp.mtype = req.pid;

        if (msgsnd(msqid, &resp, sizeof(resp) - sizeof(long), 0) < 0) {
            perror("msgsnd");
        }
    }

    return 0;
}
