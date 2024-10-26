#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <param_a> <param_b> <num_threads>"
    exit 1
fi

# Assign command-line arguments to variables
PARAM_A=$1
PARAM_B=$2
THREADS=$3

# Compile the C programs
gcc -o riemann riemann.c -lm
mpicc -o riemann_mpi riemann_mpi.c -lm
gcc -o riemann_omp riemann_omp.c -fopenmp -lm

# Run the non-MPI Riemann program and capture execution time
echo "Running riemann.c..."
START_TIME=$(date +%s.%N)
./riemann "$PARAM_A" "$PARAM_B"
END_TIME=$(date +%s.%N)
EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc)
echo "Execution time for riemann.c: $EXEC_TIME seconds"

# Run the OpenMP Riemann program and capture execution time
echo "Running riemann_omp.c..."
START_TIME=$(date +%s.%N)
./riemann_omp "$PARAM_A" "$PARAM_B" "$THREADS"
END_TIME=$(date +%s.%N)
EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc)
echo "Execution time for riemann_omp.c: $EXEC_TIME seconds"

# Run the MPI Riemann program and capture execution time
echo "Running riemann_mpi.c..."
START_TIME=$(date +%s.%N)
mpirun -np 4 ./riemann_mpi "$PARAM_A" "$PARAM_B"
END_TIME=$(date +%s.%N)
EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc)
echo "Execution time for riemann_mpi.c: $EXEC_TIME seconds"

# Clean up compiled programs
rm riemann riemann_mpi riemann_omp