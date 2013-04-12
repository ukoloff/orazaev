#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ALIVE 'X'
#define DEAD '.'

char calc_new_value(char cur_value, int num_alive) {
    if (num_alive == 3 || (num_alive == 2 && cur_value == ALIVE)) {
        return ALIVE;
    }
    return DEAD;
}

void process_row(char const * const cur_grid, char * const result, int N) {
    for (int k = 0; k < N; ++k) {
        int num_alive = 0;
        for (int i = -1; i < 2; ++i)
        for (int j = k - 1; j < k + 2; ++j) {
            if (i == 0 && j == k)
                continue;

            if (*(cur_grid + N * i + (j < 0 ? N - 1 : j % N)) == ALIVE) {
                ++num_alive;
            }
        }
        result[k] = calc_new_value(*(cur_grid + k), num_alive);
    }
}

void inspect_args(int argc, char** argv) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s [N] [input file] [number of iterations] [output file]\n", argv[0]);
        exit(1);
    }
}

void read_file(char const * const file_name, char* const data, int N) {
    FILE* input = fopen(file_name, "rb");
    if (input == 0) {
        fprintf(stderr, "ERROR: can't open file '%s' with mode 'rb'.\n", file_name);
        exit(1);
    }

    for (int i = 0; i < N; ++i) {
        fscanf(input, "%s", data + i * N);
    }

    fclose(input);
}

void write_to_file(char const * const file_name, char* const data, int N) {
    FILE* output = fopen(file_name, "wb");
    if (output == 0) {
        fprintf(stderr, "ERROR: can't open file '%s' with mode 'wb'\n", file_name);
        exit(1);
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fprintf(output, "%c", data[i * N + j]);
        }
        fprintf(output, "\n");
    }

    fclose(output);
}

void process_data(char * work_data,
        char * result,
        int * dim,
        int num_iters,
        int cur_process)
{
    /* Calc current process parameters */
    int cur_top_row = cur_process * dim[0];
    int cur_bottom_row = cur_top_row + dim[0] - 1;
    if (cur_bottom_row >= dim[1]) {
        cur_bottom_row = dim[1] - 1;
    }
    int neighbour_from_bottom = ((cur_bottom_row + 1) % dim[1]) / dim[0];
    int neighbour_from_top = cur_top_row - 1 < 0 ?
                             dim[1] / dim[0] + (dim[1] % dim[0] != 0 ? 0 : -1) :
                             (cur_top_row - 1) / dim[0];


    /* Initialize current process resources */
    char* buff = (char*) malloc(dim[1] * 4 * sizeof(char));
    MPI_Request req[4];
    memset(result, '.', dim[0] * dim[1]);

    /* Iterations cycle */
    for (int iter = 0; iter < num_iters; ++iter) {
        if (cur_top_row >= dim[1]) {
            break;
        }

        memcpy(buff + 2 * dim[1], work_data, dim[1]);
        memcpy(buff + 3 * dim[1],
               work_data + (cur_bottom_row - cur_top_row) * dim[1],
               dim[1]);

        MPI_Isend(buff + 2 * dim[1], dim[1], MPI_CHAR, neighbour_from_top,
                  0, MPI_COMM_WORLD, &req[2]);
        MPI_Isend(buff + 3 * dim[1], dim[1], MPI_CHAR,
                  neighbour_from_bottom, 0, MPI_COMM_WORLD, &req[3]);
        MPI_Irecv(buff, dim[1], MPI_CHAR, neighbour_from_top, 0,
                  MPI_COMM_WORLD, &req[0]);
        MPI_Irecv(buff + dim[1],
                  dim[1], MPI_CHAR, neighbour_from_bottom, 0,
                  MPI_COMM_WORLD, &req[1]);

        /* Process inner rows */
        for (int i = 1; i < (cur_bottom_row - cur_top_row); ++i) {
            process_row(work_data + i * dim[1], result + i * dim[1], dim[1]);
        }

        /* Process top and bottom rows when received neighbours rows */
        int complete;
        MPI_Waitany(2, req, &complete, MPI_STATUS_IGNORE);
        for (int i = 0; i < 2; ++i) {
            if (i == 1) {
                MPI_Wait(&req[complete], MPI_STATUS_IGNORE);
            }

            char* dest = work_data + (complete ?
                        (cur_bottom_row - cur_top_row + 1) * dim[1] :
                        -dim[1]);
            char* from = buff + (complete ? dim[1] : 0);
            memcpy(dest, from, dim[1]);

            if (complete) {
                process_row(work_data + (cur_bottom_row - cur_top_row) * dim[1],
                            result + (cur_bottom_row - cur_top_row) * dim[1],
                            dim[1]);
            } else {
                process_row(work_data, result, dim[1]);
            }

            complete = !complete;
        }
        MPI_Waitall(2, req + 2, MPI_STATUS_IGNORE);

        memcpy(work_data, result, dim[0] * dim[1]);
    }

    free(buff);
}

void mpi_life(int argc, char** argv) {
    inspect_args(argc, argv);

    int N = atoi(argv[1]);
    int num_iters = atoi(argv[3]);

    int num_processes;
    int cur_process;
    int root_process = 0;
    int dim[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &cur_process);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    int nrow = N + (num_processes - N % num_processes) *
          (N % num_processes != 0 ? 1 : 0);

    char* data = (char*) malloc((nrow + 2) * N * sizeof(char));

    dim[0] = nrow / num_processes; /* nrow on one processor */
    dim[1] = N; /* ncol on one processor */
    char* result = (char*) malloc(dim[0] * dim[1] * sizeof(char));

    if (cur_process == root_process) {
        read_file(argv[2], data, dim[1]);
    }

    MPI_Scatter(data, dim[0] * dim[1], MPI_CHAR,
                result, dim[0] * dim[1], MPI_CHAR,
                root_process, MPI_COMM_WORLD);
    memcpy(data + N, result, dim[1] * dim[0]);

    process_data(data + N, result, dim, num_iters, cur_process);

    MPI_Gather(result, dim[0] * dim[1], MPI_CHAR,
               data, dim[0] * dim[1], MPI_CHAR,
               root_process, MPI_COMM_WORLD);

    if (cur_process == root_process) {
        write_to_file(argv[4], data, N);
    }

    free(result);
    free(data);
    MPI_Finalize();
}

int main(int argc, char** argv) {
    mpi_life(argc, argv);
    return 0;
}
