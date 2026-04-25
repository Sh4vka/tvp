#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int main() {
    key_t key = ftok("server.c", 'A');
    int msqid = msgget(key, 0);

    if (msqid < 0) {
        perror("msgget");
        exit(1);
    }

    struct request req;
    struct response resp;

    pid_t pid = getpid();

    req.mtype = REQUEST_TYPE;
    req.pid = pid;
    snprintf(req.text, sizeof(req.text), "Hello from %d", pid);

    if (msgsnd(msqid, &req, sizeof(req) - sizeof(long), 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    if (msgrcv(msqid, &resp, sizeof(resp) - sizeof(long),
               pid, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Response for %d: %s\n", pid, resp.text);

    return 0;
}
