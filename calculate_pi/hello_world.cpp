//
// Created by 王金元 on 2022/5/12.
//

#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size, len;
    char host[128];

    // Initializes the MPI execution environment
    MPI_Init(&argc, &argv);
    //  Determines the rank of the calling process in the communicator.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Returns the size of the group associated with a communicator.
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Gets the name of the processor.
    MPI_Get_processor_name(host, &len);

    printf("Hello world from process %d of %d on %s, hostname length = %d.\n", rank, size, host, len);

    // Release MPI resources
    MPI_Finalize();

    return 0;
}