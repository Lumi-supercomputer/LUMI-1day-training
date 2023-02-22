#!/usr/bin/bash

#module load CrayEnv
module load cpe/22.08
module load cpe/22.08

#
# C-code with the GNU compilers
#

module load PrgEnv-gnu
#module unload cray-mpich

cc -O3 matrix_mult_C.c -o matrix_mult_C_gnu.x

echo -e "\nResults with the GNU C compiler:\n"

./matrix_mult_C_gnu.x 1000

#
# C code with the Cray compilers
#

module load PrgEnv-cray
#module unload cray-mpich

cc -O3 matrix_mult_C.c -o matrix_mult_C_cray.x

echo -e "\nResults with the Cray C compiler:\n"

./matrix_mult_C_cray.x 1000

#
# C code with the AOCC compilers
#

module load PrgEnv-aocc
#module unload cray-mpich

cc -O3 matrix_mult_C.c -lm -o matrix_mult_C_aocc.x

echo -e "\nResults with the AOCC C compiler:\n"

./matrix_mult_C_aocc.x 1000


#
# Fortran code with the GNU compilers
#

module load PrgEnv-gnu
#module unload cray-mpich

ftn -O3 matrix_mult_F.f90 -o matrix_mult_F_gnu.x

echo -e "\nResults with the GNU Fortran compiler:\n"

./matrix_mult_F_gnu.x 1000


#
# Fortran code with the Cray compilers
#

module load PrgEnv-cray
#module unload cray-mpich

ftn -O3 matrix_mult_F.f90 -o matrix_mult_F_cray.x

echo -e "\nResults with the Cray Fortran compiler:\n"

./matrix_mult_F_cray.x 1000



#
# Fortran code with the AOCC compilers
#

module load PrgEnv-gnu
#module unload cray-mpich

ftn -O3 matrix_mult_F.f90 -o matrix_mult_F_aocc.x

echo -e "\nResults with the GNU Fortran compiler:\n"

./matrix_mult_F_aocc.x 1000

