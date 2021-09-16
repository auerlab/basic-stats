#!/bin/sh -e

cd ..
./cave-man-install.sh
ldd basic-stats
cd Test
set -x
../basic-stats \
    mean --col 1 \
    median --col 1 \
    mean --row 1 \
    median --row 1 \
    mean --col 3 \
    median --col 2 \
    < sample.txt
