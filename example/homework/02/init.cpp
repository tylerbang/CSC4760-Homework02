using namespace std;
#include <mpi.h>
#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>

// haha funni comment

// this is main function
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    // get that rank and size
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // variables for the number of rows, columns, and time steps
    int nRows;
    int nCols;
    int nTime;

    // Check the number of arguments
    if (rank == 0) {
        if (argc != 4) {
            cout << "Usage: mpirun -n <number of processes> ./init <nRows> <nCols> <nTime>" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        nRows = atoi(argv[1]);
        nCols = atoi(argv[2]);
        nTime = atoi(argv[3]);
        if (nRows < 1 || nCols < 1 || nTime < 1) {
            cout << "nRows, nCols, nTime must be greater than 0" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Broadcast the number of rows, columns, and time steps
    MPI_Bcast(&nRows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nCols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nTime, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Check if the number of rows is divisible by the number of processes
    int nRowsLocal = nRows / size;
    if (rank == (size-1)){
       nRowsLocal += nRows % size; 
    }

    // Initialize the domain
    int nRowsLocalWithGhost = nRowsLocal + 2;
    int nColsWithGhost = nCols + 2;

    vector<vector<int>> currDomain(nRowsLocalWithGhost, vector<int>(nColsWithGhost, 0));
    vector<vector<int>> nextDomain(nRowsLocalWithGhost, vector<int>(nColsWithGhost, 0));

    // fill the domain with random values
    srand(time(0));
    for (int iRow = 1; iRow <= nRowsLocal; iRow++){
        for (int iCol = 1; iCol <= nColsWithGhost; iCol++){
            currDomain[iRow][iCol] = rand() % 2;
        }
    }

    int upperNeighbor = (rank == 0) ? size - 1 : rank - 1;
    int lowerNeighbor = (rank == size - 1) ? 0 : rank + 1;
    
    const int ALIVE = 1;
    const int DEAD = 0;
    
    // Main loop
    for (int iTime = 0; iTime < nTime; iTime++){
        MPI_Send(&currDomain[1][0], nColsWithGhost, MPI_INT, upperNeighbor, 0, MPI_COMM_WORLD);
        MPI_Send(&currDomain[nRowsLocal][0], nColsWithGhost, MPI_INT, lowerNeighbor, 0, MPI_COMM_WORLD);

        MPI_Recv(&currDomain[nRowsLocal + 1][0], nColsWithGhost, MPI_INT, lowerNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&currDomain[0][0], nColsWithGhost, MPI_INT, upperNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int iRow = 0; iRow < nRowsLocalWithGhost; iRow++){
            currDomain[iRow][0] = currDomain[iRow][nCols];
            currDomain[iRow][nCols + 1] = currDomain[iRow][1];
        }

        if (rank != 0){
            for (int iRow = 1; iRow <= nRowsLocal; iRow++){
                MPI_Send(&currDomain[iRow][1], nCols, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        if (rank == 0){
            cout << "iTime: " << iTime << endl;

            for (int iRow = 1; iRow <= nRowsLocal; iRow++){
                for (int iCol = 1; iCol <= nCols; iCol++){
                    cout << currDomain[iRow][iCol] << " ";
                }
                cout << endl;
            }
            for (int sourceRank = 1; sourceRank < size; sourceRank++){
                int nRecv = nRows / size;
                if (sourceRank == size - 1){
                    nRecv += nRows % size;
                }
                vector<int> buff(nCols, 0);
                for (int iRecv = 0; iRecv < nRecv; iRecv++){
                    MPI_Recv(&buff[0], nCols, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    for (int i : buff){
                        cout << i << " ";
                    }
                    cout << endl;
                }
            }
        }
        
                // Update the next domain
        for (int iRow = 1; iRow <= nRowsLocal; iRow++){
            for (int iCol = 1; iCol <= nCols; iCol++){
                int nAliveNeighbors = 0;
                for (int jRow = max(1, iRow - 1); jRow <= min(nRowsLocal, iRow + 1); jRow++){
                    for (int jCol = max(1, iCol - 1); jCol <= min(nCols, iCol + 1); jCol++){
                        if (((jRow != iRow) || (jCol != iCol)) && currDomain[jRow][jCol] == ALIVE){
                            nAliveNeighbors++;
                        }
                    }
                }

                if (currDomain[iRow][iCol] == ALIVE){
                   nextDomain[iRow][iCol] = (nAliveNeighbors == 2 || nAliveNeighbors == 3) ? ALIVE : DEAD;
                } else {
                     nextDomain[iRow][iCol] = (nAliveNeighbors == 3) ? ALIVE : DEAD;
                }
            }
        }

        for (int iRow = 1; iRow <= nRowsLocal; iRow++){
            for (int iCol = 1; iCol <= nCols; iCol++){
                currDomain[iRow][iCol] = nextDomain[iRow][iCol];
            }
        }
    }
    MPI_Finalize();    
    return 0;
}
/*
⢰⣶⠶⢶⣶⣶⡶⢶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⡶⠶⢶⣶⣶⣶⣶
⠘⠄⠄⠄⠄⠄⠄⠄⠄⣿⣿⣿⣿⣿⣿⣿⠿⠄⠄⠄⠈⠉⠄⠄⣹⣶⣿⣿⣿⣿⢿
⠄⠤⣾⣿⣿⣿⣿⣷⣤⡈⠙⠛⣿⣿⣿⣧⣀⠠⣤⣤⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣶
⢠⠄⠄⣀⣀⣀⣭⣿⣿⣿⣶⣿⣿⣿⣿⣿⣿⣤⣿⣿⠉⠉⠉⢉⣉⡉⠉⠉⠙⠛⠛
⢸⣿⡀⠄⠈⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⢿⣿⣿⣷⣾⣿
⢸⣿⣿⣿⣿⣿⣿⣿⣿⠛⢩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⢸⣿⣿⣿⣿⣿⡿⣿⣿⣴⣿⣿⣿⣿⣄⣠⣴⣿⣷⣭⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⠸⠿⣿⣿⣿⠋⣴⡟⠋⠈⠻⠿⠿⠛⠛⠛⠛⠛⠃⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⢸⣿⣿⣿⡁⠈⠉⠄⠄⠄⠄⠄⣤⡄⠄⠄⠄⠄⠄⠈⠄⠈⠻⠿⠛⢿⣿⣿⣿⣿⣿
⢸⣿⣿⣿⠄⠄⠄⠄⠄⠄⠄⠄⣠⣄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⢀⣀⣿⣿⣿⣿
⢸⣿⣿⣿⡀⠄⠄⠄⠄⠄⠄⠄⠉⠉⠁⠄⠄⠄⠄⠐⠒⠒⠄⠄⠄⠄⠉⢸⣿⣿⣿
⢸⣿⣿⣿⢿⣦⣄⣠⣄⠛⠟⠃⣀⣀⡀⠄⠄⣀⣀⣀⣀⣀⣀⡀⢀⣰⣦⣼⣿⣿⡿
⢸⣿⣿⣿⣿⣿⣿⣻⣿⠄⢰⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢛⣥⣾⣟⣿⣿⣿⣿⣿
⢸⣿⣿⣿⣿⣿⣿⣿⣿⡆⠈⠿⠿⠿⠿⠿⠿⠿⠿⠿⣧⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿
⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
*/
