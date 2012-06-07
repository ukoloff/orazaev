#!/usr/bin/env bash

make clean > /dev/null 2>&1
make > test_build.log 2>&1 || exit 1

./utest

rm test_build.log
