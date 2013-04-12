#!/usr/bin/env bash
# set -x

. common_definitions.sh

INPUT_FILE="$PWD/data/input.grid"
CANONIC_RESULT="data/canonic.grid"
CUR_RESULT="$PWD/data/result.grid"

DATA_DIM=""
ITER_NUMBER=""

function GetTime() {
    python -c "import time; print time.time();"
}

function DiffTime() {
    python -c "print $1 - $2"
}

function Compiling() {
    echo "Compiling."
    mkdir -p bin
    $CC $DATA_GEN_SRC -o $DATA_GEN
    $CC $LIFE_SRC -o $LIFE2

    $MPICC $MPI_LIFE_SRC -o $MPI_LIFE
}

function GenerateRandomData() {
    mkdir -p ./data

    DATA_DIM=$((RANDOM % 50 + 1))
    ITER_NUMBER=$((RANDOM % 50 + 1))
    echo "Generating random grid with N = $DATA_DIM."

    $DATA_GEN $DATA_DIM $INPUT_FILE

    echo "$DATA_DIM" > ./data/dim.var
    echo "$ITER_NUMBER" > ./data/iterations.var
}

function GenerateBigRandomData() {
    mkdir -p ./data

    DATA_DIM=2000
    ITER_NUMBER=$((RANDOM % 50 + 1))
    echo "Generating random grid with N = $DATA_DIM."

    $DATA_GEN $DATA_DIM $INPUT_FILE

    echo "$DATA_DIM" > ./data/dim.var
    echo "$ITER_NUMBER" > ./data/iterations.var
}

function CalcCanonicResult() {
    echo "Calculating canonic grid using $LIFE2, iterations = $ITER_NUMBER."
    $LIFE2 $DATA_DIM $INPUT_FILE $ITER_NUMBER $CANONIC_RESULT
}

function Cleanup() {
    echo "Cleanup."
    rm -rf ./bin
    rm -rf ./data
}


function CalcMpiLife() {
    local NODES=2
    local PPN=2
    if [ $# -gt 0 ]; then
        NODES=$1
        PPN=$2
    fi
    echo "Starting mpi calculation."
    local try=0

    while [ $try -lt 3 ]; do
        local ok="TRUE"

        local ID=`qsub -l nodes=$NODES:ppn=$PPN ./start_mpi_life.sh`
        number=`echo $ID | sed 's/\..*//g'`
        local err=start_mpi_life.sh.e$number
        local out=start_mpi_life.sh.o$number

        echo "Enqueing new task with ID = $ID."
        while [ ! -e $out ]; do
            sleep 1
        done
        echo -n "Task complete "

        if [ "`cat $err | wc -l`" -ne 0 ]; then
            echo "with errors:"
            head -5 $err
            echo ""

            ok=""
            try=$((try + 1))
        else
            echo "without errors."
        fi

        if diff $CANONIC_RESULT $CUR_RESULT > /dev/null; then
            echo "Result: [PASS]"
            local TIMING=`cat $out`
            printf "csv_data: $NODES,$PPN,$TIMING\n"
        else
            echo "Result: [CKSUM]"
            printf "csv_data: $NODES,$PPN,NA\n"
        fi

        rm -f $err
        rm -f $out

        if [ ! "$ok" ]; then
            echo "Trying again..."
            continue
        fi

        break
    done
}

function Test() {
    local repeats=1
    if [ $# -gt 0 ]; then
        repeats=$1
    fi
    Compiling

    while [ $repeats -gt 0 ]; do
        GenerateRandomData
        CalcCanonicResult
        CalcMpiLife

        repeats=$((repeats - 1))
    done
}

function Timing() {
    local repeats=1
    if [ $# -gt 0 ]; then
        repeats=$1
    fi

    Compiling
    GenerateBigRandomData
    printf "csv_data: NumNodes,NumProcessors,Time\n"

    local I=$repeats
    while [ $I -gt 0 ]; do
        local startTime=`GetTime`
        CalcCanonicResult > /dev/null
        local endTime=`GetTime`
        printf "csv_data: 1,1,`DiffTime $endTime $startTime`\n"

        I=$((I - 1))
    done

    for nnodes in 1 2 3; do
    for nproc in 2 3; do
        I=$repeats
        while [ $I -gt 0 ]; do
            CalcMpiLife $nnodes $nproc
            I=$((I -1))
        done
    done
    done
}


FUNCTION_NAME=$1
shift
$FUNCTION_NAME $@
echo "Complete."
