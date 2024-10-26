# Riemann Integral Computation 📐

## Overview

This project implements the Riemann integral computation using three different parallelization strategies:
1. **Serial Implementation** (`riemann.c`): A straightforward Riemann integration using the trapezoidal rule.
2. **MPI Implementation** (`riemann_mpi.c`): A parallelized version of the Riemann integration using the Message Passing Interface (MPI) for distributed computing.
3. **OpenMP Implementation** (`riemann_omp.c`): A parallelized version utilizing OpenMP for shared memory multiprocessing.

## Description

The Riemann integral computes the area under a curve defined by a mathematical function. This project specifically implements the following functions:
- \( f(x) = x^2 \) 
- \( g(x) = 2x^3 \) 
- \( h(x) = \sin(x) \) 

The trapezoidal rule is used to approximate the integral over a specified interval `[a, b]` by dividing it into smaller segments (trapezoids) and summing their areas.

## Requirements ⚙️

### Compiling the Code

To compile the code, ensure you have the following installed:
- GCC (GNU Compiler Collection) 🛠️
- OpenMPI 🌐
- OpenMP (included with GCC) 🖥️

### Compiling Instructions

Run the following commands in your terminal:

```bash
gcc -o riemann riemann.c -lm
mpicc -o riemann_mpi riemann_mpi.c -lm
gcc -o riemann_omp riemann_omp.c -fopenmp -lm
```

### Executing the Programs

You can execute the programs using the provided shell script, `run_programs.sh`, which simplifies the process of running each implementation and capturing execution times.

### Shell Script Instructions

1. **Make the script executable**:
   ```bash
   chmod +x run_programs.sh
   ```

2. **Run the script with the required parameters**:
   ```bash
   ./run_programs.sh <param_a> <param_b> <num_threads>
   ```

   For example:
   ```bash
   ./run_programs.sh 0 10 4
   ```

## Parameters

- **param_a**: The lower limit of the integral (e.g., `0`).
- **param_b**: The upper limit of the integral (e.g., `1`).
- **num_threads**: The number of threads for the OpenMP implementation (e.g., `4`).

## Output

Each implementation will output the approximation of the integral along with the execution time for that computation. The execution times for each program will also be logged in execution_times.csv.

Example output:
```
F(x) = (x)^2, N = 100,000,000; 
Running riemann.c...
Running riemann_omp.c...
Running riemann_mpi.c...
```

The execution_times.csv file will contain:

```
Program,Execution Time (seconds)
riemann,0.123456
riemann_omp,0.098765
riemann_mpi,0.076543
```

If you want to see the detailed results of the integrals, you may run each program separately (i.e., riemann, riemann_omp, and riemann_mpi) as they provide comprehensive output specific to the integral calculations.

## Performance Comparison

This project allows for the comparison of execution times across different implementations. Depending on the number of trapezoids and the number of processes/threads, you may observe significant differences in performance.

## Author

- [Andrea-gt](https://github.com/Andrea-gt)