#!/usr/bin/env bash

make clean > /dev/null 2>&1
make > test_build.log 2>&1 || exit 1

valgrind ./utest_nodelist

rm test_build.log
