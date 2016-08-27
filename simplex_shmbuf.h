#ifndef SIMPLEX_SHMBUF_H
#define SIMPLEX_SHMBUF_H

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h> // mmap
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_NAME "/testing"

#define BUFSZ 128 

/*
 * simplex (adj.): allowing communication in only one direction at a time 
 *
 * assumptions: one reader, one writer, one direction of communication
 */
struct simplex_shmbuf {
    char buf[BUFSZ];

    /* a simple 'switch' that is flipped back and forth to indicate that the
     * data in the buffer is ready to be consumed by the reader 
     */
    volatile bool ready;
    size_t ready_len;
};

struct simplex_shmbuf *
sshmbuf_create(const char *const shm_name)
{
    shm_unlink(SHM_NAME); /* unlink if exists */

    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror(__func__);
        return NULL;
    }

    if (ftruncate(fd, sizeof(struct simplex_shmbuf)) < 0) {
        perror(__func__);
        return NULL;
    }
    void *ptr = mmap(NULL,
                     sizeof(struct simplex_shmbuf),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd,
                     0);
    if (ptr == MAP_FAILED) {
        perror(__func__);
        return NULL;
    }

    /* note that 'ready' is initialized to false */
    memset(ptr, 0, sizeof(struct simplex_shmbuf));

    return ptr; 
}

struct simplex_shmbuf *
sshmbuf_lookup(const char *const shm_name)
{
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        printf("%s: couldn't open %s: %s\n", __func__, shm_name, strerror(errno));
        return NULL;
    }

    void *ptr = mmap(NULL,
                     sizeof(struct simplex_shmbuf),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd,
                     0);

    if (ptr == MAP_FAILED) {
        perror(__func__);
        return NULL;
    }

    return ptr;
}

#endif // SIMPLEX_SHMBUF_H
