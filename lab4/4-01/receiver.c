#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define TERMINATOR_TYPE 255

struct message {
    long mtype;
    char m[256];
};

int main() {
    key_t key = ftok("sender.c", 'A');
    int msqid = msgget(key, 0);

    struct message msg;

    while (1) {
        msgrcv(msqid, &msg, sizeof(msg.m), 0, 0);

        if (msg.mtype == TERMINATOR_TYPE) {
            msgctl(msqid, IPC_RMID, NULL);
            break;
        }

        printf("type=%ld text=%s\n", msg.mtype, msg.m);
    }

    return 0;
}
