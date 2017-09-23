#!/bin/bash -eu
if [[ -z ${RUN_FROM_MAKEFILE:-} ]]; then
    echo "only run via \`make run\`"
    exit 1
fi

# Start prog1, the 
./prog1 &
p1_pid=$!
sleep 1
(cat shmem_sendbuf.h | ./prog2) || echo "failed"
kill $p1_pid
