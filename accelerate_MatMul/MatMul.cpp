//
// Created by 王金元 on 2022/5/16.
// This code is for matrix calculation with MPI
//

#include <mpi.h>
#include "utils.hpp"
#include <thread>

// processes with rank > 0 are used for calculating
void calculator_process(
        const int rank,
        const int root_rank,
        const int world_size
) {
    MetaInfo meta_info;

    // Receive meta info from broadcasting
    MPI_Bcast(&meta_info, sizeof(MetaInfo), MPI_CHAR, root_rank, MPI_COMM_WORLD);

    // Calculate slice size for current calculator
    auto slice_size = BLOCK_SIZE(rank, world_size, meta_info.second_col_size);

//    // Declare matrices for receiving data
    Eigen::MatrixXd matrix_1(meta_info.first_row_size, meta_info.first_col_size);
    Eigen::MatrixXd matrix_2_slice(meta_info.second_row_size, slice_size);
    Eigen::MatrixXd result_matrix(meta_info.first_row_size, slice_size);

    // Receive the second matrix from broadcasting
    MPI_Bcast(matrix_1.data(),
              meta_info.first_row_size * meta_info.first_col_size,
              MPI_DOUBLE,
              root_rank,
              MPI_COMM_WORLD
    );

    // Receive the first matrix slice
    MPI_Recv(matrix_2_slice.data(),
             slice_size * meta_info.second_row_size,
             MPI_DOUBLE,
             root_rank,
             SLICE_MSG,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE
    );

    // Calculate
    result_matrix = matrix_1 * matrix_2_slice;

    // Send the result to root rank
    MPI_Send(result_matrix.data(),
             slice_size * meta_info.second_row_size,
             MPI_DOUBLE,
             root_rank,
             RESULT_MSG,
             MPI_COMM_WORLD
    );
}


// Process root rank is the allocator
void allocator_process(
        int rank,
        int world_size,
        MetaInfo &meta_info,
        Eigen::MatrixXd &matrix_1,
        Eigen::MatrixXd &matrix_2,
        Eigen::MatrixXd &result_matrix
) {

    // Each calculator has a request of result
    auto requests = new MPI_Request[world_size-1];


    // Broadcast meta info to calculator ranks
    MPI_Bcast(&meta_info,
              sizeof(MetaInfo),
              MPI_CHAR,
              rank,
              MPI_COMM_WORLD
    );

    // Broadcast the first matrix to calculator ranks
    MPI_Bcast(matrix_1.data(),
              meta_info.first_col_size * meta_info.first_row_size,
              MPI_DOUBLE,
              rank,
              MPI_COMM_WORLD
    );


    // Set up a timer
    auto t_begin = MPI_Wtime();

    // Allocate compute tasks to each calculator process
    for (int cal_rank = 1; cal_rank < world_size; cal_rank++) {
        auto block_low = BLOCK_LOW(cal_rank, world_size, meta_info.second_col_size);
        auto block_size = BLOCK_SIZE(cal_rank, world_size, meta_info.second_col_size);
        Eigen::MatrixXd matrix_2_slice = matrix_2(Eigen::all, Eigen::seqN(block_low, block_size));

        // Send matrix 2 slices to calculators
        MPI_Send(matrix_2_slice.data(),
                 block_size * meta_info.second_row_size,
                 MPI_DOUBLE,
                 cal_rank,
                 SLICE_MSG,
                 MPI_COMM_WORLD
        );

        // Receive results from calculators
        MPI_Irecv(result_matrix.data() + block_low * meta_info.second_row_size,
                  block_size * meta_info.second_row_size,
                  MPI_DOUBLE,
                  cal_rank,
                  RESULT_MSG,
                  MPI_COMM_WORLD,
                  &requests[cal_rank-1]
        );
    }

    // Wait for all requests to be received
    MPI_Waitall(world_size-1, requests, MPI_STATUS_IGNORE);

    // Calculate time
    auto total_time = MPI_Wtime() - t_begin;
    printf("[Rank 0] World size: %d\tcalculators: %d\tTotal time: %0.5fs\n", world_size, world_size - 1, total_time);

    // Free requests
    delete[] requests;
}


void single_process(
        MetaInfo &meta_info,
        Eigen::MatrixXd &matrix_1,
        Eigen::MatrixXd &matrix_2,
        Eigen::MatrixXd &result_matrix
) {
    // Set up a timer
    auto t_begin = MPI_Wtime();
    // Calculate
    result_matrix = matrix_1 * matrix_2;
    // Calculate time
    auto total_time = MPI_Wtime() - t_begin;
    printf("[Rank 0] World size: %d\tcalculators: %d\tTotal time: %0.5fs\n", 1, 1, total_time);
}


int main(int argc, char **argv) {
    int rank, world_size;
    int root_rank = std::stoi(argv[1]);

    MPI_Init(&argc, &argv);

    //Get process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //Get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (root_rank > world_size || root_rank == world_size) {
        printf("[Error] Root rank: %d must be smaller than world size: %d\n", root_rank, world_size);
        return 1;
    }

    if (rank == root_rank) {
        const std::string first_path(argv[2]);
        const std::string second_path(argv[3]);
        const std::string result_path(argv[4]);

        // Meta info of 2 matrices
        MetaInfo meta_info;
        // Declare 2 matrices to load
        Eigen::MatrixXd matrix_1;
        Eigen::MatrixXd matrix_2;

        // Load 2 matrices from file
        load_matrix_from_file(first_path, matrix_1, meta_info.first_row_size, meta_info.first_col_size);
        load_matrix_from_file(second_path, matrix_2, meta_info.second_row_size, meta_info.second_col_size);

        // Declare the result matrix according to existing info
        Eigen::MatrixXd result_matrix(meta_info.first_row_size, meta_info.second_col_size);

        // Set up synchronization barrier
        MPI_Barrier(MPI_COMM_WORLD);

        if (world_size == 1) {
            // Directly calculate when single process
            single_process(meta_info, matrix_1, matrix_2, result_matrix);
        }
        else {
            // Run allocator when multiple processes
            allocator_process(rank, world_size, meta_info, matrix_1, matrix_2, result_matrix);
        }

        // Dump the result matrix
        auto error = dump_matrix_to_file(result_path,
                                         result_matrix,
                                         meta_info.first_row_size,
                                         meta_info.second_col_size);
        if (error) std::cout << "Error when dumping result to: " << result_path << std::endl;

        // Set up synchronization barrier
        MPI_Barrier(MPI_COMM_WORLD);
    }
    else {
        // Set up synchronization barrier
        MPI_Barrier(MPI_COMM_WORLD);

        // Run calculator process on other ranks
        calculator_process(rank, root_rank, world_size);

        // Set up synchronization barrier
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}




