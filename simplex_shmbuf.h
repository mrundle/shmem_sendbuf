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

/*
 * simplex (adj.): allowing communication in only one direction at a time 
 *
 * assumptions: one reader, one writer, one direction of communication
 */

static void *
shm_create(const char *const shm_name, size_t size)
{
    shm_unlink(shm_name); /* unlink if exists */

    int fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

static void *
shm_lookup(const char *const shm_name, size_t size)
{
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        printf("%s: couldn't open %s: %s\n", __func__, shm_name, strerror(errno));
        return NULL;
    }

    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED) {
        perror(__func__);
        return NULL;
    }

    return ptr;
}

#define SSHMBUF_DEFINE(name, elt_type, n_elt)                                  \
struct name {                                                                  \
    elt_type buf[n_elt * sizeof(elt_type)];                                    \
    volatile bool ready;                                                       \
    size_t ready_len;                                                          \
};                                                                             \
                                                                               \
struct name *                                                                  \
name ## _create(const char *const shm_name)                                    \
{                                                                              \
    void *ptr = shm_create(shm_name, sizeof(struct name));                     \
    if (ptr) {                                                                 \
        memset(ptr, 0, sizeof(struct name));                                   \
    }                                                                          \
    return ptr;                                                                \
}                                                                              \
                                                                               \
struct name *                                                                  \
name ## _lookup(const char *const shm_name)                                    \
{                                                                              \
    return shm_lookup(shm_name, sizeof(struct name));                          \
}

SSHMBUF_DEFINE(char_sshmbuf, char, 128)

#endif // SIMPLEX_SHMBUF_H
