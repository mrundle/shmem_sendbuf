/*
 * This program looks up a shared memory zone, and writes the
 * contents of STDIN to it in chunks. This will be picked up
 * by prog1 and written to that programs STDOUT.
 */
#include "prog_common.h"

#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    struct char_shmem_sendbuf *shmbuf =
        shmbuf_lookup(EXAMPLE_SHM_NAME, sizeof(*shmbuf));
    if (shmbuf == NULL) {
        return -EIO;
    }

    /* write STDIN to the other process */
    ssize_t nread;
    while ((nread = read(STDIN_FILENO,
                         &shmbuf->buf,
                         sizeof(shmbuf->buf))) > 0) {
        shmbuf->ready_len = nread;
        shmbuf->ready = true;
        while (shmbuf->ready == true) {
            usleep(100);
        }
    }
    shmbuf->ready_len = 0;
    shmbuf->ready = (nread == 0);

    return 0;
}
