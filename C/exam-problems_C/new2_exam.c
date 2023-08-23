#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_FILENAME_LEN 1024
#define MAX_NUM_FDS 1024

typedef struct {
        int num_fds;
        int fds[MAX_NUM_FDS];
} fd_ctx_t;

void init_ctx(fd_ctx_t* ctx) {
        ctx->num_fds = 0;
}

void close_pipes(fd_ctx_t* ctx) {
        for (int i = 0; i < ctx->num_fds; i++) {
                close(ctx->fds[i]);
        }
}

pid_t spawn_dir_walker(char* dirname, int out_fd, fd_ctx_t* ctx) {
        pid_t pid = fork();
        if (pid < 0) {
                err(1, "could not fork");
        }
        if (pid != 0) {
                return pid;
        }

        if (dup2(out_fd, 1) < 0) {
                err(1, "could not dup");
        }
        close_pipes(ctx);

        execlp(
                "find",
                "find", dirname,
                "-type", "f", "-not", "-name", "*.hash",
                (char*)NULL
        );
        err(1, "could not exec find");
}

bool file_exists(char* filename) {
        struct stat statbuf;
        int stat_err = stat(filename, &statbuf);
        if (stat_err == 0) {
                return true;
        }
        if (stat_err < 0 && errno != ENOENT) {
                err(1, "could not stat %s", filename);
        }
        // errno is ENOENT, file does not exist
        return false;
}

pid_t spawn_hasher(char* filename, fd_ctx_t* ctx) {
        pid_t pid = fork();
        if (pid < 0) {
                err(1, "could not fork");
        }
        if (pid != 0) {
                return pid;
        }

        close_pipes(ctx);

        // compute name of output file
        int max_out_filename_len = MAX_FILENAME_LEN + strlen(".hash");
        char out_filename[max_out_filename_len];
        strncpy(out_filename, filename, MAX_FILENAME_LEN);
        strncat(out_filename, ".hash", max_out_filename_len);

        if (file_exists(out_filename)) {
                exit(0);        // skip processing already-present file
        }

        int out_fd = open(out_filename, O_WRONLY | O_CREAT, 0666);
        if (out_fd < 0) {
                err(1, "could not open hash file (%s)", out_filename);
        }

        if (dup2(out_fd, 1) < 0) {
                err(1, "could not dup");
        };

        execlp(
                "md5sum",
                "md5sum", filename,
                (char*)NULL
        );
        err(1, "could not exec find");
}

void make_pipe(fd_ctx_t* ctx, int pfd[2]) {
        if (pipe(pfd) < 0) {
                err(1, "could not create pipe");
        }

        if (ctx->num_fds >= MAX_NUM_FDS) {
                errx(1, "cannot create more file descriptors");
        }

        ctx->fds[ctx->num_fds] = pfd[0];
        ctx->fds[ctx->num_fds + 1] = pfd[1];
        ctx->num_fds += 2;
}

bool read_filename(int fd, char* filename) {
        for (size_t i = 0; i < MAX_FILENAME_LEN; i++) {
                int read_size = read(fd, &filename[i], 1);
                if (read_size < 0) {
                        err(1, "could not read filename");
                }
                if (read_size == 0) {
                        if (i != 0) {
                                errx(1, "incomlete data from find");
                        }
                        return false;
                }
                if (filename[i] == '\n') {
                        filename[i] = '\0';
                        return true;
                }
        }
        errx(1, "find gave us a longer filename than we can handle");
}

bool wait_for_child(void) {
        int wait_status;
        int result = wait(&wait_status);
        if (result < 0) {
                if (errno == ECHILD) {
                        return false;
                }
                err(1, "could not wait");
        }
        if (WIFEXITED(wait_status)) {
                if (WEXITSTATUS(wait_status) == 0) {
                        // exited successfully
                        return true;
                }
                errx(1, "child failed");
        }
        errx(1, "child was killed");
}

int main(int argc, char* argv[]) {
        if (argc != 2) {
                warnx("usage: %s <dir>", argv[0]);
                errx(1, "%s computes the hashes of the files in the given dir in parallel", argv[0]);
        }

        fd_ctx_t ctx;
        init_ctx(&ctx);

        int walker_pipe[2];
        make_pipe(&ctx, walker_pipe);

        spawn_dir_walker(argv[1], walker_pipe[1], &ctx);
        close(walker_pipe[1]);

        char filename[MAX_FILENAME_LEN];
        while (read_filename(walker_pipe[0], filename)) {
                spawn_hasher(filename, &ctx);
        }

        // wait for all children
        while(wait_for_child());

        close_pipes(&ctx);

        return 42;
}
