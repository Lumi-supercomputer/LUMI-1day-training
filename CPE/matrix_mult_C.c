/* C-implementation of matrix-matrix multiplication.
** Based on C++-code used by Intel in demo sessions.
*/

#define ALIGNMENT 64

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <cblas.h>

double flops( int size, double time ) {

    return ( 2. * (double) size * (double) size * (double) size) / time;

}

double readTime() {

    struct timeval tp;

    gettimeofday( &tp, NULL );
    return (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6;

}

int main(int argc, char** argv) {

    double *A;
    double *B;
    double *C;
    int size;
    double start, end;
    double walltime;
    double dummy = 0.;

    printf( "Matrix multiplication demo, multiplying two square matrices using:\n" );
    printf( " - Naive sequential code: 6 possible orders of the three loops\n" );
    printf( " - BLAS\n\n" );

    /* Read the size of the matrix (first argument) */

    if ( argc != 2 ) {
        printf( "The program requires one command line arguments: the matrix size.\n" );
        return -1;
    }

    size      = atoi( argv[1] );

    printf( "Matrix multiplication demo, multiplying two square matrices of size %d.\n", size );

    /* Allocate memory. We do an aligned allocation for optimal BLAS-performance. */
    A = (double *) aligned_alloc( ALIGNMENT, size*size*sizeof(double) );
    B = (double *) aligned_alloc( ALIGNMENT, size*size*sizeof(double) );
    C = (double *) aligned_alloc( ALIGNMENT, size*size*sizeof(double) );

    /* Initialize the matrices A and B. */
    for ( int i = 0; i < size; i++ )
        for ( int j = 0; j < size; j++ ) {
            A[i*size+j] = sin( (double) (i+j) / (double) size * 3.1415 );
            B[i*size+j] = cos( (double) (i+j) / (double) size * 3.1415 );
        }

    /* Do the actual computations.
     * After each algorithm, partial results are printed.
     */
    printf( "Matrix size: %dx%d, %.3f MB per array.\n", size, size, (double) (8*size*size) / (1024.0*1024.0) );
    printf( "Variant       time (s)    Gflops  control sum\n" );
    printf( "---------------------------------------------\n" );

    /* ijk-loop */
    start = readTime();
    for ( int i = 0; i < size; i++ )
        for ( int j = 0; j < size; j++ ) {
            C[i*size+j] = 0.;
            for ( int k = 0; k < size; k++ )
                C[i*size+j] += A[i*size+k] * B[k*size+j];
        }
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "ijk-variant:  %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* jik-loop */
    start = readTime();
    for ( int j = 0; j < size; j++ )
        for ( int i = 0; i < size; i++ ) {
            C[i*size+j] = 0.;
            for ( int k = 0; k < size; k++ )
                C[i*size+j] += A[i*size+k] * B[k*size+j];
        }
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "jik-variant:  %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* ikj-loop */
    start = readTime();
    for ( int i = 0; i < size; i++ ) {
        for ( int j = 0; j < size; j++ ) C[i*size+j] = 0.;
        for ( int k = 0; k < size; k++ )
            for ( int j = 0; j < size; j++ )
                C[i*size+j] += A[i*size+k] * B[k*size+j];
    }
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "ikj-variant:  %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* kij-loop */
    start = readTime();
    for ( int i = 0; i < size; i++ )
        for ( int j = 0; j < size;j++ ) C[i*size+j] = 0.;
    for ( int k = 0; k < size; k++ ) {
        for ( int i = 0; i < size; i++ )
            for ( int j = 0; j < size; j++ )
                C[i*size+j] += A[i*size+k] * B[k*size+j];
    }
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "kij-variant:  %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* kji-loop */
    start = readTime();
    for ( int i = 0; i < size; i++ )
        for ( int j = 0; j < size; j++ ) C[i*size+j] = 0.;
    for ( int k = 0; k < size; k++ ) {
        for ( int j = 0; j < size; j++ )
            for ( int i = 0; i < size; i++ )
                C[i*size+j] += A[i*size+k] * B[k*size+j];
    }
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "kji-variant:  %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* jki-loop */
    start = readTime();
    for ( int j = 0; j < size; j++ ) {
        for ( int i = 0; i < size; i++ ) C[i*size+j] = 0.;
        for ( int k = 0; k < size; k++ )
            for ( int i = 0; i < size; i++ )
                C[i*size+j] += A[i*size+k] * B[k*size+j];
    }
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "jki-variant:  %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* CBLAS */
    start = readTime();
    double alpha = 1.0;
    double beta  = 0.0;
    cblas_dgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size, alpha, A, size,
                 B, size, beta,C, size );
    end = readTime();
    walltime = end - start;
    dummy = 0.; for ( int i = 0; i < size; i++ ) for ( int j = 0; j < size; j++ ) dummy += C[i*size+j];
    printf( "BLAS:         %8.2f  %8.2f  %11.3e\n", walltime, flops( size, walltime ) / 1.e9, dummy );

    /* Print the matrices in MATLAB-compatible code for debugging if size <= 5. */
    if ( size <= 5 ) {

    	printf( "\n\n" );

    	printf( "A = [ ...\n" );
        for ( int i = 0; i < size; i++ ) {
        	for ( int j = 0; j < size; j++ )
        		printf( "%23.16e ", A[i*size+j]);
        	printf( "; ...\n" );
        }
        printf( "];\n" );

    	printf( "B = [ ...\n" );
        for ( int i = 0; i < size; i++ ) {
        	for ( int j = 0; j < size; j++ )
        		printf( "%23.16e ", B[i*size+j]);
        	printf( "; ...\n" );
        }
        printf( "];\n" );

    	printf( "C = [ ...\n" );
        for ( int i = 0; i < size; i++ ) {
        	for ( int j = 0; j < size; j++ )
        		printf( "%23.16e ", C[i*size+j]);
        	printf( "; ...\n" );
        }
        printf( "];\n" );

    }

    return 0;

}
