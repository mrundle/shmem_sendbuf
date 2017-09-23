CFLAGS = -Werror -Wall
LDFLAGS = -lrt
PROGRAMS = prog1 prog2

all: $(PROGRAMS)

prog%: prog%.c prog_common.h shmem_sendbuf.h
	gcc $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: run
run: run.sh $(PROGRAMS)
	RUN_FROM_MAKEFILE=1 ./$<

clean:
	rm -f $(PROGRAMS)
