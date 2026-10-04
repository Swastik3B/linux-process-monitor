#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

    int fd;
    char buffer[100];

    fd = open("/dev/process_monitor", O_RDONLY);

    if (fd < 0) {
        perror("Unable to open process monitor device");
        return 1;
    }

    int bytes = read(fd, buffer, sizeof(buffer) - 1);

    if (bytes < 0) {
        perror("Unable to read from device");
        close(fd);
        return 1;
    }

    buffer[bytes] = '\0';

    printf("\n====================================\n");
    printf("       DEVICE DRIVER TEST\n");
    printf("====================================\n");
    printf("Driver message: %s", buffer);
    printf("====================================\n");

    close(fd);

    return 0;
}
