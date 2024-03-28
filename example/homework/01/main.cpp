#include <mpi.h>
#include <iostream>

// main method
int main (int argc, char **argv) {
    // initialize that MPI thing
    MPI_Init(&argc, &argv);

    // le size
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // thing to be passed around
    int token = 0;
    // if not the first process, receive token from previous process
    if (rank != 0){
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received token " << token << " from process " << (rank-1) << std::endl;
    }
    // send token to next process
    token++;
    MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    // if the first process, receive token from last process
    if (rank == 0){
        MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received token " << token << " from process " << size - 1 << std::endl;
    }

    // close that MPI boi
    MPI_Finalize();
    return 0;
}