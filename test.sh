#!/bin/sh -e

./cave-man-install.sh
ldd ./basic-stats
./basic-stats \
    mean --col 1 \
    median --col 1 \
    mean --row 1 \
    median --row 1 \
    mean --col 3 \
    median --col 2 \
    < sample.txt
