//
// This program is to calculate PI with MPI.
// The algorithm is based on integral representation of PI.
// PI is the integral of 4/(1+x^2) for x from 0 to 1.
// Created by NoeWang on 2022/5/12.
//

#include "mpi.h"
#include <iostream>

#define TOTAL_STEPS 2E9
#define USE_MPI_REDUCE true
#define USE_MPI_SEND_RECV true


double calculate_pi_by_MPI_Reduce(int rank, int size){
    int i;
    double partial_sum = 0, total_sum = 0, result=0.0, t_begin, t_end;
    const long long total_steps = TOTAL_STEPS;
    const double scale_factor = 1 / ((double)total_steps * (double)total_steps);

    //Synchronize all processes and get the beginning time
    MPI_Barrier(MPI_COMM_WORLD);
    t_begin = MPI_Wtime();

    for (i = rank; i < total_steps; i += size)
    {
        partial_sum += 4.0 / (1.0 + scale_factor * i * i);
    }

    //Sum up all results
    MPI_Reduce(&partial_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    t_end = MPI_Wtime();

    if (rank==0)
    {
        result = total_sum / total_steps;
        printf("[MPI Reduce]\tProcesses = %2d;\tTime = %fs;\tPI = %0.10lf\n", size, t_end-t_begin, result);
    }

    return result;
}


double calculate_pi_by_MPI_SendRecv(int rank, int size){
    int i;
    double partial_sum = 0, total_sum = 0, result=0.0, t_begin, t_end;
    const long long total_steps = TOTAL_STEPS;
    const double scale_factor = 1 / ((double)total_steps * (double)total_steps);
    auto* requests = new MPI_Request[size-1];

    //Synchronize all processes and get the beginning time
    MPI_Barrier(MPI_COMM_WORLD);
    t_begin = MPI_Wtime();


    for (i = rank; i < total_steps; i += size) {
        partial_sum += 4.0 / (1.0 + scale_factor * i * i);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank==0) {
        double partial_sum_received;
        total_sum = partial_sum;
        for (int i_rank=1; i_rank<size; i_rank++){
            MPI_Recv(&partial_sum_received, 1, MPI_DOUBLE, i_rank, i_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += partial_sum_received;
        }
        result = total_sum / total_steps;
    }
    else {
        MPI_Isend(&partial_sum, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, &requests[rank]);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    t_end = MPI_Wtime();
    if (rank == 0) {
        printf("[MPI SendRecv]\tProcesses = %2d;\tTime = %fs;\tPI = %0.10lf\n", size, t_end-t_begin, result);
    }
    delete[] requests;

    return result;
}



int main(int argc, char** argv){

    int rank, size;

    MPI_Init(&argc, &argv);

    //Get process ID
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Get processes Number
    MPI_Comm_size(MPI_COMM_WORLD, &size);

#if USE_MPI_REDUCE
    calculate_pi_by_MPI_Reduce(rank, size);
#endif

#if USE_MPI_SEND_RECV
    calculate_pi_by_MPI_SendRecv(rank, size);
#endif

    MPI_Finalize();

}

