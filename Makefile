CFLAGS = -Werror -Wall
LDFLAGS = -lrt
PROGRAMS = prog1 prog2

all: $(PROGRAMS)

prog%: prog%.c prog_common.h shmem_sendbuf.h
	gcc $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: run
run: $(PROGRAMS)
	./prog1 & \
	p1_pid=$! \
	cat shmem_sendbuf.h | ./prog2 \
	kill $$p1_pid

clean:
	rm -f $(PROGRAMS)
