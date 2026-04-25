#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define TERMINATOR_TYPE 255

struct message {
    long mtype;
    struct {
        int id;
        float value;
    } data;
};

int main() {
    key_t key = ftok("sender.c", 'A');
    if (key < 0) {
        perror("ftok");
        exit(1);
    }

    int msqid = msgget(key, 0);
    if (msqid < 0) {
        perror("msgget");
        exit(1);
    }

    struct message msg;

    while (1) {
        ssize_t len = msgrcv(msqid, &msg, sizeof(msg.data), 0, 0);
        if (len < 0) {
            perror("msgrcv");
            exit(1);
        }

        if (msg.mtype == TERMINATOR_TYPE) {
            printf("Terminator received\n");
            msgctl(msqid, IPC_RMID, NULL);
            break;
        }

        printf("Received: id=%d value=%.2f\n",
               msg.data.id, msg.data.value);
    }

    return 0;
}
