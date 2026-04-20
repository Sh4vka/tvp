#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    printf("Reader: opening FIFO...\n");

    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char buffer[100];

    read(fd, buffer, sizeof(buffer));

    printf("Reader got: %s\n", buffer);

    close(fd);
    return 0;
}
