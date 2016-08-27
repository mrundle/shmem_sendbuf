#ifndef SHMEM_SENDBUF_H
#define SHMEM_SENDBUF_H

/* assumptions: one reader, one writer; one direction of communication */

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFINE_SHMEM_SENDBUF(name, elt_type, n_elt) \
struct name {                                       \
    elt_type buf[n_elt * sizeof(elt_type)];         \
    volatile bool ready;                            \
    size_t ready_len;                               \
};

static inline void *
shmbuf_create(const char *const name, size_t size)
{
    shm_unlink(name); /* unlink if exists */

    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror(__func__);
        return NULL;
    }

    if (ftruncate(fd, size) < 0) {
        perror(__func__);
        return NULL;
    }
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror(__func__);
        return NULL;
    }

    return ptr;
}

static inline void *
shmbuf_lookup(const char *const name, size_t size)
{
    int fd = shm_open(name, O_RDWR, 0);
    if (fd == -1) {
        printf("%s: couldn't open %s: %s\n", __func__, name, strerror(errno));
        return NULL;
    }

    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED) {
        perror(__func__);
        return NULL;
    }

    return ptr;
}

#endif // SHMEM_SENDBUF_H
