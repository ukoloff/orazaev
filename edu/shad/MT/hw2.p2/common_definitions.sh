#!/usr/bin/env bash

DATA_GEN_SRC=src/data-gen.c
LIFE_SRC=src/life2.c
MPI_LIFE_SRC=src/mpi_life.c

DATA_GEN=./bin/data-gen.out
LIFE2=./bin/life2.out
MPI_LIFE=$PWD/bin/mpi_life.out



CC='gcc -std=c99'
MPICC='mpicc -std=c99 -Wall --pedantic'
