#!/usr/bin/bash

#module load CrayEnv
module load cpe/22.08
module load cpe/22.08

#
# GNU compilers
#

module load PrgEnv-gnu

cc -O3 -fopenmp mpi_omp_hello.c -o mpi_omp_hello_gnu.x


#
# Cray compilers
#

module load PrgEnv-cray

cc -O3 -fopenmp mpi_omp_hello.c -o mpi_omp_hello_cray.x


#
# AMD AOCC compilers
#

module load PrgEnv-aocc

cc -O3 -fopenmp mpi_omp_hello.c -o mpi_omp_hello_aocc.x
