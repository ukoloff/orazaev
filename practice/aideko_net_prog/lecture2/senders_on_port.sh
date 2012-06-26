#!/usr/bin/env bash

if [ $# -lt 1 ]; then
    echo "You bust set port number as script arg." 1>&2
    exit 1
fi

./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &
./sender.out $1 &

wait
exit 0
