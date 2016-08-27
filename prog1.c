#include "simplex_shmbuf.h"

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    struct simplex_shmbuf *sshmbuf = sshmbuf_create(SHM_NAME);
    if (sshmbuf == NULL) {
        return -EIO;
    }

    while (true) {
        while (sshmbuf->ready == false) {
            usleep(100);
        }
        write(STDOUT_FILENO, &sshmbuf->buf, sshmbuf->ready_len);
        sshmbuf->ready = false;
    } 

    // TODO catch kill signal and unlink the shm

    return 0;
}
