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


int main(int argc, char** argv) {
    int N;
    int num_iters;
    int num_processes;
    int cur_process;
    int root_process = 0;
    int dim[2];
    int i, j;

    if (argc < 5) {
        fprintf(stderr, "Usage: %s [N] [input file] [number of iterations] [output file]\n", argv[0]);
        exit(1);
    }
    N = atoi(argv[1]);
    num_iters = atoi(argv[3]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &cur_process);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    int nrow = N + (num_processes - N % num_processes) *
          (N % num_processes != 0 ? 1 : 0);
    int ncol = N;

    char* data = (char*) malloc((nrow + 2) * ncol * sizeof(char));
    char* buff = (char*) malloc(ncol * 4 * sizeof(char));
    dim[0] = nrow / num_processes; /* nrow on one processor */
    dim[1] = ncol; /* ncol on one processor */
    char* result = (char*) malloc(dim[0] * dim[1] * sizeof(char));

    if (cur_process == root_process) {
        FILE* input = fopen(argv[2], "r");
        if (input == 0) {
            fprintf(stderr, "ERROR: can't open file '%s'\n", argv[2]);
            exit(1);
        }
        for (i = 0; i < dim[1]; ++i) {
            fscanf(input, "%s", data + i * dim[1]);
        }
        fprintf(stderr, "ERROR: can't open file '%s'\n", argv[2]);

        fclose(input);
    }

    char* work_data = data + N;
    int count = nrow * ncol / num_processes;
    MPI_Scatter(data, count, MPI_CHAR,
                result, count, MPI_CHAR,
                root_process, MPI_COMM_WORLD);
    memcpy(work_data, result, dim[1] * dim[0]);

    MPI_Request req[4];
    for (int iter = 0; iter < num_iters; ++iter) {
        int cur_top_row = cur_process * dim[0];
        int cur_bottom_row = cur_top_row + dim[0] - 1;
        int neighbour_from_top, neighbour_from_bottom;
        if (cur_bottom_row >= N) {
            cur_bottom_row = N - 1;
        }

        neighbour_from_bottom = ((cur_bottom_row + 1) % N) / dim[0];
        neighbour_from_top = cur_top_row - 1 < 0 ?
                             N / dim[0] + (N % dim[0] != 0 ? 0 : -1) :
                             (cur_top_row - 1) / dim[0];

        if (cur_top_row >= N) {
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

        memset(result, '.', dim[0] * dim[1]);
        for (i = 1; i < (cur_bottom_row - cur_top_row); ++i) {
            fprintf(stderr, "[%d] %d %c%c\n", cur_process, i,
                    *(work_data + i * dim[1]),
                    *(work_data + i * dim[1] + 1));
            process_row(work_data + i * dim[1], result + i * dim[1], dim[1]);
        }

        int complete;
        MPI_Waitany(2, req, &complete, MPI_STATUS_IGNORE);
        for (i = 0; i < 2; ++i) {
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

        //for (i = 0; i <= (cur_bottom_row - cur_top_row); ++i) {
        //    printf("[%d] ", cur_process);
        //    for (j = 0; j < dim[1]; ++j)
        //        printf("%c", *(result + i * dim[1] + j));
        //    printf("\n");
        //}
    }


    MPI_Gather(result, count, MPI_CHAR,
               data, count, MPI_CHAR,
               root_process, MPI_COMM_WORLD);

    if (cur_process == root_process) {
        FILE* output = fopen(argv[4], "wb");
        if (output == 0) {
            fprintf(stderr, "ERROR: can't open file '%s' with mode 'wb'\n", argv[4]);
            exit(1);
        }
        for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            fprintf(output, "%c", data[i * N + j]);
        }
            fprintf(output, "\n");
        }
        fclose(output);
    }

    free(result);
    free(buff);
    free(data);
    MPI_Finalize();
    return 0;
}
