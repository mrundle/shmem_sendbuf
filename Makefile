CFLAGS = -Werror -Wall
LDFLAGS = -lrt
PROGRAMS = prog1 prog2

all: $(PROGRAMS)

prog%: prog%.c simplex_shmbuf.h
	gcc $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: run
run: $(PROGRAMS)
	./prog1 & \
	p1_pid=$! \
	cat simplex_shmbuf.h | ./prog2 \
	kill $$p1_pid

clean:
	rm -f $(PROGRAMS)
