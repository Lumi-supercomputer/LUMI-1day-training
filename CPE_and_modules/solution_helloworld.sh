#!/usr/bin/bash

#module load CrayEnv
module load cpe/22.08
module load cpe/22.08

#
# GNU compilers
#

module load PrgEnv-gnu

cc hello_world.c -o hello_world_c_gnu.x
CC hello_world.cc -o hello_world_cc_gnu.x
ftn hello_world.f -o hello_world_f_gnu.x
ftn hello_world.f90 -o hello_world_f90_gnu.x

#
# Cray compilers
#

module load PrgEnv-cray

cc hello_world.c -o hello_world_c_cray.x
CC hello_world.cc -o hello_world_cc_cray.x
ftn hello_world.f -o hello_world_f_cray.x
ftn hello_world.f90 -o hello_world_f90_cray.x

#
# AMD AOCC compilers
#

module load PrgEnv-aocc

cc hello_world.c -o hello_world_c_aocc.x
CC hello_world.cc -o hello_world_cc_aocc.x
ftn hello_world.f -o hello_world_f_aocc.x
ftn hello_world.f90 -o hello_world_f90_aocc.x
