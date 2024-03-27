#include <mpi.h>
#include <iostream>

// main method
int main (int argc, char **argv) {
    // initialize that MPI thing
    MPI_Init(&argc, &argv);

    // le size
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // le rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // thing to be passed around
    int token;
    // if not the first process, receive token from previous process
    if (rank != 0){
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received token " << token << " from process " << size - 1 << std::endl;
    } else {
        // else, set token to 5
        token = 5;
    }
    // send token to next process
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