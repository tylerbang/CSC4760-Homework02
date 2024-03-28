using namespace std;

#include <mpi.h>
#include <iostream>

/*
    3. Demonstrate the use of MPI Alltoall to send a personalized communication between each process in MPI COMM WORLD.
*/

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf[size];

    for (int i = 0; i < size; i++) {
        sendbuf[i] = rank;
    }

    int recvbuf[size];

    MPI_Alltoall(&sendbuf, 1, MPI_INT, &recvbuf, 1, MPI_INT, MPI_COMM_WORLD);

    cout << "Process " << rank << " received: ";
    for (int i = 0; i < size; i++) {
        cout << recvbuf[i] << " ";
    }
    cout << endl;
    
    MPI_Finalize();
    return 0;
}