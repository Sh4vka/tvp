#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);

    zmq_bind(socket, "tcp://*:5555");

    char buffer[256];

    printf("ZMQ server started...\n");

    while (1) {
        // получаем запрос
        zmq_recv(socket, buffer, 256, 0);
        printf("Received: %s\n", buffer);

        // формируем ответ
        time_t now = time(NULL);
        char reply[256];
        snprintf(reply, sizeof(reply), "Time: %s", ctime(&now));

        // отправляем ответ
        zmq_send(socket, reply, strlen(reply), 0);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}
