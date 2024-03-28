using namespace std;

#include <mpi.h>
#include <iostream>

/*
    2. Demonstrate the use of MPI Gather and MPI Bcast to achieve the same result as MPI Allgather.
*/

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int recvbuf[size];
    int result[size];

    MPI_Gather(&rank, 1, MPI_INT, &recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Bcast(recvbuf, size, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(result, size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            cout << recvbuf[i] << " ";
        }
        cout << endl;
    } else {
        for (int i = 0; i < size; i++) {
            cout << result[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}