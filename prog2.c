#include "simplex_shmbuf.h"

#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    struct char_sshmbuf *sshmbuf = char_sshmbuf_lookup(SHM_NAME);
    if (sshmbuf == NULL) {
        return -EIO;
    }

    /* write STDIN to the other process */
    ssize_t nread;
    while ((nread = read(STDIN_FILENO,
                         &sshmbuf->buf,
                         sizeof(sshmbuf->buf))) > 0) {
        sshmbuf->ready_len = nread;
        sshmbuf->ready = true;
        while (sshmbuf->ready == true) {
            usleep(100);
        }
    }
    sshmbuf->ready_len = 0;
    sshmbuf->ready = (nread == 0);

    return 0;
}
