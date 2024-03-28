using namespace std;

#include <mpi.h>
#include <iostream>

/*
    1. Demonstrate the use of MPI Bcast and MPI Reduce to achieve the same result as MPI Allreduce.
*/

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int result = 0;
    int thing_to_sum = 45;

    MPI_Reduce(&thing_to_sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Process: " << rank << " Result: " << result << endl;
    } else {
        cout << "Process: " << rank << " Result: " << result << endl;
    }
    MPI_Finalize();
    return 0;   
}