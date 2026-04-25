#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERMINATOR_TYPE 255
#define NORMAL_TYPE 1

struct message {
    long mtype;
    struct {
        int id;
        float value;
    } data;
};

int main() {
    key_t key = ftok("sender.c", 'A');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    struct message msg;

    for (int i = 1; i <= 5; i++) {
        msg.mtype = NORMAL_TYPE;
        sprintf(msg.m, "Message #%d", i);

        msgsnd(msqid, &msg, strlen(msg.m)+1, 0);
    }

    msg.mtype = TERMINATOR_TYPE;
    msgsnd(msqid, &msg, 0, 0);

    return 0;
}
