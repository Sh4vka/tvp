#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    mkfifo(FIFO_NAME, 0666);

    printf("Writer: opening FIFO...\n");

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char msg[] = "Hello from writer!";

    write(fd, msg, strlen(msg) + 1);

    printf("Writer: message sent\n");

    close(fd);
    return 0;
}
