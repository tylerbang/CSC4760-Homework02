using namespace std;
#include <mpi.h>
#include <iostream>
#include <assert.h>

void zero_domain(char *domain, int M, int N);
void print_domain(char *domain, int M, int N);
void update_domain(char *new_domain, char *old_domain, int M, int N);

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();
    return 0;
}