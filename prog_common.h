#ifndef PROG_COMMON_H
#define PROG_COMMON_H

#include "shmem_sendbuf.h"

DEFINE_SHMEM_SENDBUF(char_shmem_sendbuf, char, 32)

#define EXAMPLE_SHM_NAME "/simplex_shmbuf_example"

#endif // PROG_COMMON_H
