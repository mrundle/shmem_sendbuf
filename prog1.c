/*
 * This program creates a shared memory zone, and then watches
 * it, printing out anything that it finds there.
 */
#include "prog_common.h"

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    struct char_shmem_sendbuf *shmbuf =
        shmbuf_create(EXAMPLE_SHM_NAME, sizeof(*shmbuf));

    if (shmbuf == NULL) {
        return -EIO;
    }

    memset(shmbuf, 0, sizeof(*shmbuf));

    while (true) {
        while (shmbuf->ready == false) {
            usleep(100);
        }
        write(STDOUT_FILENO, &shmbuf->buf, shmbuf->ready_len);
        shmbuf->ready = false;
    } 

    // TODO catch kill signal and unlink the shm

    return 0;
}
