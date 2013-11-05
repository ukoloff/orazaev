/**
 * Exercise 5-2 from "Linux programming interface"
 *
 * Author: Aman Orazaev.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_APPEND | O_CREAT | O_RDWR, S_IRWXU);

    if (fd == -1) {
        perror("Can't open given file with O_APPEND flag");
        exit(1);
    }

    if (write(fd, "..........", 10) == -1) {
        perror("Can't write to file.");
        exit(1);
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        exit(1);
    }

    if (write(fd, "HELLO!", 6) == -1) {
        perror("Can't write to file.");
        exit(1);
    }

    if (close(fd) == -1) {
        perror("Can't close file.");
        exit(1);
    }

    return 0;
}
