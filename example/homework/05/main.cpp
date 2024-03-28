using namespace std;

#include <mpi.h>
#include <iostream>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int data_for_color1 = 4, data_for_color2 = 5;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int P = 2, Q = size / 2;

    if (P * Q != size){
        cout << "Size of MPI_COMM_WORLD is not exactly P*Q" << endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int color1 = rank / Q;
    MPI_Comm comm1;
    MPI_Comm_split(MPI_COMM_WORLD, color1, rank, &comm1);

    int color2 = rank % Q;
    MPI_Comm comm2;
    MPI_Comm_split(MPI_COMM_WORLD, color2, rank, &comm2);

    int rank1, size1;
    MPI_Comm_rank(comm1, &rank1);
    MPI_Comm_size(comm1, &size1);

    int rank2, size2;
    MPI_Comm_rank(comm2, &rank2);
    MPI_Comm_size(comm2, &size2);

    MPI_Bcast(&data_for_color1, 1, MPI_INT, 0, comm1);
    MPI_Bcast(&data_for_color2, 1, MPI_INT, 0, comm2);

    cout << "Rank: " << rank << " Color1: " << color1 << " Rank1: " << rank1 << " Size1: " << size1 << " Data1: " << data_for_color1 << endl;
    cout << "Rank: " << rank << " Color2: " << color2 << " Rank2: " << rank2 << " Size2: " << size2 << " Data2: " << data_for_color2 << endl;

    MPI_Comm_free(&comm1);
    MPI_Comm_free(&comm2);
    MPI_Finalize();
    return 0;
}