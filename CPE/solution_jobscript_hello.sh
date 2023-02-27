#! /bin/bash
#SBATCH -J demo_mpi_omp
#SBATCH -p standard
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=4
#SBATCH --hint=nomultithread

module load cpe/22.08
module load cpe/22.08
module load PrgEnv-aocc

echo "Running on $(hostname)"

echo -e "\n\nRunning the binary compiled with the GNU compilers...\n"
srun ./mpi_omp_hello_gnu.x

echo -e "\n\nRunning the binary compiled with the Cray compilers...\n"
srun ./mpi_omp_hello_cray.x

echo -e "\n\nRunning the binary compiled with the AOCC compilers...\n"
srun ./mpi_omp_hello_aocc.x
