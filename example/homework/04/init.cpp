using namespace std;

#include <mpi.h>
#include <iostream>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Finalize();
    return 0;   
}