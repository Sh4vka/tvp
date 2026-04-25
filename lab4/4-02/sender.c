#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define NORMAL_TYPE 1
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

    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid < 0) {
        perror("msgget");
        exit(1);
    }

    struct message msg;

    // отправка 5 сообщений
    for (int i = 0; i < 5; i++) {
        msg.mtype = NORMAL_TYPE;
        msg.data.id = i;
        msg.data.value = i * 1.5f;

        if (msgsnd(msqid, &msg, sizeof(msg.data), 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        printf("Sent: id=%d value=%.2f\n",
               msg.data.id, msg.data.value);
    }

    // сообщение-терминатор
    msg.mtype = TERMINATOR_TYPE;
    msgsnd(msqid, &msg, 0, 0);

    return 0;
}
