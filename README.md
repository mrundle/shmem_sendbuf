# simplex_shmbuf
[C] A lockless, unidirectional, shared memory IPC buffer.

prog1: Creates a POSIX shared memory zone, writes anything it
       finds there to STDOUT (forever).

prog2: Looks up the POSIX shared memory zone created by prog1,
       writes the contents of STDIN to that zone in bursts.
       prog1 will write this to stdout.

run.sh: Run via `make run`, this program demonstrates how this
        all works. It starts prog1 as a background process,
        then cats a text file (simplex_shmbuf.h) into ./prog2.
        This data will be transferred from prog2 to prog1 over
        shared memory and written to the screen (prog1's STDOUT).

To build and run an example:
<code>
make run
</code>
