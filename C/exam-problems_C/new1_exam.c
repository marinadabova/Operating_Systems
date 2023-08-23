
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

bool read_byte(int in_fd, uint8_t* result) {
        int n = read(in_fd, result, sizeof(*result));
        if (n == 0) {
                return false;
        }
        if (n < 0) {
                err(1, "could not read byte");
        }
        return true;
}

bool read_data(int in_fd, uint8_t* data, uint8_t N) {
        int n = read(in_fd, data, N);
        if (n < 0) {
                err(1, "could not read data");
        }
        if (n < N) {
                return false;
        }
        return true;
}

void write_byte(int out_fd, uint8_t byte) {
        int n = write(out_fd, &byte, sizeof(byte));
        if (n < 0) {
                err(1, "error while writing byte");
        }
        if (n != sizeof(byte)) {
                errx(1, "could not write byte to output file");
        }
}

void write_data(int out_fd, uint8_t* data, int exp_n) {
        int n = write(out_fd, data, exp_n);
        if (n < 0) {
                err(1, "error while writing data");
        }
        if (n != exp_n) {
                errx(1, "could not write data to output file at once");
        }
}

void copy_if_msg(int in_fd, int out_fd) {
        uint8_t start, N, expected_checksum, data[256];

        bool read_result = read_byte(in_fd, &start);
        if (!read_result || start != 0x55) {
                return;
        }
        read_result = read_byte(in_fd, &N);
        if (N < 3 || !read_result) {
                warnx("skipped message due to corrupted size byte");
                return;
        }
        if (!read_data(in_fd, data, N - 3)) {
                warnx("skipped message due to incomplete data");
                return;
        }
        read_result = read_byte(in_fd, &expected_checksum);
        if (!read_result) {
                return;
        }

        uint8_t checksum = start ^ N;
        for (uint8_t i = 0; i < N - 3; i++) {
                checksum ^= data[i];
        }
        if (expected_checksum != checksum) {
                warnx("skipped message due to corrupted checksum");
                return;
        }

        // we have a valid message

        dprintf(1, "valid message!\n");
        write_byte(out_fd, start);
        write_byte(out_fd, N);
        write_data(out_fd, data, N - 3);
        write_byte(out_fd, checksum);
}

int seek(int fd, off_t offset, int whence) {
        int result = lseek(fd, offset, whence);
        if (result < 0) {
                err(1, "could not determine file size");
        }
        return result;
}

int main(int argc, char* argv[]) {
        if (argc != 3) {
                warnx("usage: %s <input stream> <output file>", argv[0]);
                warnx("%s extracts the valid messages from a stream", argv[0]);
                errx(1, "required 3 arguments");
        }

        int in_fd = open(argv[1], O_RDONLY);
        if (in_fd < 0) {
                err(1, "could not open %s", argv[1]);
        }

        int out_fd = open(
                argv[2],
                O_WRONLY | O_TRUNC | O_CREAT,
                0666
        );
        if (out_fd < 0) {
                err(1, "could not open %s", argv[2]);
        }

        int file_size = seek(in_fd, 0, SEEK_END);

        for (int i = 0; i < file_size - 3; i++) {
                seek(in_fd, i, SEEK_SET);
                copy_if_msg(in_fd, out_fd);
        }

        close(in_fd);
        close(out_fd);
}
