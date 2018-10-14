#!/bin/bash -eu

# make sure we have our programs
ls ./prog1 ./prog2 >/dev/null

SRC=$(mktemp)
DST=$(mktemp)
PROG1_PID=0

cleanup() {
    rm -f $SRC $DST
    [[ $PROG1_PID -ne 0 ]] && kill $PROG1_PID
}
trap cleanup EXIT

cat <<EOF > $SRC
This test file will be piped into prog2
and written out by prog1.
EOF

# Start the listener, prog1, in the background,
# writing its stdout to DST
./prog1 > $DST &
PROG1_PID=$!

# Let prog1 initialize
sleep 1

# cat the SRC file through prog2, write stdout to DST
cat $SRC | ./prog2

diff $SRC $DST >/dev/null && pass=true || pass=false
printf "TEST "
$pass && echo SUCCESS || echo FAILED
trap - EXIT
$pass
