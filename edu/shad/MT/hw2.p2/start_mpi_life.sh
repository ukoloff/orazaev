#!/usr/bin/env bash

PREFIX=/home/orazaev/git/orazaev/edu/shad/MT/hw2.p2

mpirun $PREFIX/bin/mpi_life.out `cat $PREFIX/data/dim.var` $PREFIX/data/input.grid `cat $PREFIX/data/iterations.var` $PREFIX/data/result.grid
