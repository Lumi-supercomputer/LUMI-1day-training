//
// Demo program part of the UAntwerp VSC tutorials.
//
// This program is used to demonstrate starting a hybrid OpenMP/MPI-program.
// It is essentially a hello world program, but with an added element that does
// make the source code more difficult: To make it easy to check that all processes
// and threads run where we expect it, we make sure that the output is ordered
// according to MPI rank and thread number.
//

#define _GNU_SOURCE   // Needed to get gethostname from unistd.h and sched_getcpu from sched.h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

#include <mpi.h>
#include <omp.h>

#define HOSTNAMELENGTH 40              // Not really good practice to fix the length of strings as it can cause
                                       // buffer overflows (though not in this case), but let's not make it harder
                                       // than needed given the purpose of this program.

typedef struct {
    int  *corenum;                     // Will point to an array containing the OS core number for each OpenMP thread
                                       // We'll do a dirty trick and actually start that array at the firstcore field
                                       // when we allocate memory.
    int  mpi_myid;                     // MPI rank of the current process
    int  openmp_numthreads;            // Number of threads in the current MPI process
    char mpi_hostname[HOSTNAMELENGTH]; // Name of the host on which this process is running
    int  firstcore;                    // To make sure that the end of the struct is properly aligned for integers.
} t_rankData;

/******************************************************************************
This is a simple hello world program. Each thread prints out the rank of its
MPI process and its OpenMP thread number, and the total number of MPI ranks
and OpenMP threads per process.
******************************************************************************/

int main( int argc, char *argv[] )
{
    int mpi_myid, mpi_numprocs;
    int openmp_numthreads;              // Number of OpenMP threads.
    int max_openmp_numthreads;          // Maximal number of threads per process over all MPI processes.
    int sum_openmp_numthreads;          // Total number of OpenMP threads over all MPI processes.
    t_rankData *my_rankData;            // Rank data for this MPI process
    t_rankData *buf_rankData;           // Buffer to receive data from another process.
    int my_rankData_size, buf_rankData_size;
    int error;
    MPI_Request request;
    MPI_Status status;

    MPI_Init( &argc, &argv );            // Standard way of initilizing a MPI program.

    //
    // Initializations
    //

    // Get basic info: Number of processes and the number of threads in this process.
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_numprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_myid );

#pragma omp parallel shared( openmp_numthreads )   // The shared clause is not strictly needed as that variable will be shared by default.
    { if ( omp_get_thread_num() == 0 ) openmp_numthreads = omp_get_num_threads(); } // Must be in a parallel session to get the proper number.

    MPI_Reduce( &openmp_numthreads, &max_openmp_numthreads, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD );
    MPI_Reduce( &openmp_numthreads, &sum_openmp_numthreads, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );

    // Print some information on the data just computed.
    if ( mpi_myid == 0 )
        printf( "Running a total of %d threads in %d MPI ranks (maximum %d threads per rank).\n",
                sum_openmp_numthreads, mpi_numprocs, max_openmp_numthreads );

    // Now create the t_rankData data structures.

    my_rankData_size = sizeof( t_rankData ) + (openmp_numthreads - 1) * sizeof( int );
    my_rankData = (t_rankData *) malloc( my_rankData_size );
    if ( my_rankData == NULL ) { fprintf( stderr, "ERROR: Memory allocation for my_rankData failed.\n" ); return 1; };
    my_rankData->corenum = &(my_rankData->firstcore);

    if ( mpi_myid == 0 ) {
        buf_rankData_size = sizeof( t_rankData ) + (max_openmp_numthreads - 1) * sizeof( int );
        buf_rankData = (t_rankData *) malloc( buf_rankData_size );
        if ( buf_rankData == NULL ) { fprintf( stderr, "ERROR: Memory allocation for buf_rankData failed.\n" ); return 2; };
        buf_rankData->corenum = &(buf_rankData->firstcore);
    }  // end if ( mpi_myid == 0 )

    //
    // Gather all data for the current MPI process and store in the my_rankData structure.
    //

    my_rankData->mpi_myid = mpi_myid;
    my_rankData->openmp_numthreads = openmp_numthreads;
    gethostname( my_rankData->mpi_hostname, HOSTNAMELENGTH );
#pragma omp parallel
    {
        int openmp_myid;
        int cpunum;

        /* Get OpenMP information. */
        openmp_myid = omp_get_thread_num();
        my_rankData->corenum[openmp_myid] = sched_getcpu();
    } // End of OpenMP parallel section.

    //
    // Finally print the results.
    // We'll do all printing on the master thread of MPI rank 0 so that
    // the output is sorted as it should.
    // To avoid having to build complex MPI datatypes, we assume a homogeneous
    // cluster in the sense that all nodes used have the same CPU family.
    //

    // Send data to process with rank 0.
    error = MPI_Isend( my_rankData, my_rankData_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &request );

    if ( mpi_myid == 0 ) {

        printf( "++ Output format:             (MPIrank,thread) of (#MPIprocs,#threads in rank) on cpu <cpu> of <host>\n" );

        for ( int c1 = 0; c1 < mpi_numprocs; c1++ ) {

            error = MPI_Recv( buf_rankData, buf_rankData_size, MPI_BYTE, c1, 0, MPI_COMM_WORLD, &status );
            buf_rankData->corenum = &(buf_rankData->firstcore); // Because the pointer from the other process doesn't make sense.

            for ( int c2 = 0; c2 < buf_rankData->openmp_numthreads; c2++ ) {
                printf( "++ MPI rank/OpenMP thread num (    %3d,   %3d) of (      %3d,             %3d) on cpu   %3d of %s\n",
                        buf_rankData->mpi_myid, c2, mpi_numprocs, buf_rankData->openmp_numthreads,
                        buf_rankData->corenum[c2], buf_rankData->mpi_hostname );

            }  // end for ( int c2...
        }  // end for ( int c1...

    } // end if ( mpi_myid == 0 )


    // Close off properly.

    MPI_Barrier( MPI_COMM_WORLD );
    MPI_Finalize();

    return 0;

}

