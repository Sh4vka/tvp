#include <zmq.h>
#include <stdio.h>
#include <string.h>

int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);

    zmq_connect(socket, "tcp://localhost:5555");

    char request[256] = "Hello from client";

    zmq_send(socket, request, strlen(request), 0);

    char reply[256];
    zmq_recv(socket, reply, 256, 0);

    printf("Reply: %s\n", reply);

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}
