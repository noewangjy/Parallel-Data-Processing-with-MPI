//
// Created by 王金元 on 2022/5/12.
// This code is for prime sieve
// We only consider odd numbers
//

#include <mpi.h>
#include <numeric>
#include "utils.cpp"



#define SQUARE(x) ((x)*(x))


void exit_on_error(){
    MPI_Finalize();
    exit(1);
}



int main(int argc, char** argv) {
    unsigned long upper_bound, upper_bound_copy;
    int rank, world_size;
    double t_start, t_end;
    char* marked_array;

    // Block info
    unsigned long block_low;
    unsigned long block_high;
    unsigned long block_size;

    // Temp variables
    unsigned long prime;      // current prime
    unsigned long index;      // index of current prime
    unsigned long first;      // index of first prime
    unsigned long local_count; // local count of primes
    unsigned long global_count; // global count of primes




    MPI_Init(&argc, &argv);
    //  Determines the rank of the calling process in the communicator.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Returns the size of the group associated with a communicator.
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (argc != 2) {
        printf("Incorrect number of arguments. Expected command:\n $ %s  <int: N>", argv[0]);
        exit_on_error();
    }
    upper_bound = atoi(argv[1]);
    upper_bound_copy = upper_bound;
    upper_bound = (upper_bound % 2 == 0) ? (upper_bound / 2 - 1) : ((upper_bound - 1) / 2);

    MPI_Barrier(MPI_COMM_WORLD);

    block_size = BLOCK_SIZE(rank, world_size, upper_bound);
    block_high = BLOCK_HIGH(rank, world_size, upper_bound);
    block_low = BLOCK_LOW(rank, world_size, upper_bound);


    marked_array = (char*) calloc(block_size, 1);

    if (marked_array == nullptr){
        printf("[Rank %d] Failed to allocate enough memory!\n", rank);
        exit_on_error();
    }
    t_start = MPI_Wtime();

    // ============= Sieving =================
    if (rank == 0) index = 0;
    prime = 3; // since we consider only adds, we start from 3, but DO NOT forget 2
    do {
        if (SQUARE(prime) > block_low){
            first = (SQUARE(prime) - block_low) / 2;
        }
        else {
            if (!(block_low % prime)) {
                first = 0;
            }
            else {
                first = ((prime - block_low%prime)%2 == 0) ?
                        ((prime - block_low%prime)/2) : ((prime - block_low % prime + prime) / 2);
            }
        }

        // Mark multiples of primes in array
        for (auto i=first; i<block_size; i+=prime) marked_array[i] = 1;
        if (rank==0){
            while (marked_array[++index]);
            prime = 2*index + 3;
        }
        // Broadcast the prime found to all processes
        if (world_size > 1) {
            MPI_Bcast(&prime, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        }
    } while (SQUARE(prime) <= 1+ 2 * upper_bound);

    local_count = block_size - std::accumulate(marked_array, marked_array+block_size, 0);

    if (world_size > 1){
        MPI_Reduce(&local_count, &global_count, 1,
                   MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    else {
        global_count = local_count;
    }
    t_end = MPI_Wtime();

    global_count += 1; // Attention! number 2 is a prime, but we start from 3.

    if (rank == 0){
        printf("[Rank %d] There are %ld primes smaller than %ld\tProcessors: %d\tTime: %.5fs\n",
               rank, global_count, upper_bound_copy, world_size, t_end - t_start);
    }


    MPI_Finalize();

    return 0;
}
