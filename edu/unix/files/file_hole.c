/**
 * Just trying to write after the end of the file.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    lseek(fd, 10, SEEK_CUR);
    write(fd, "Hello!", 6);
    close(fd);

    return 0;
}
