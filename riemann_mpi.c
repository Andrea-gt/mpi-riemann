/**
 * @file riemann_mpi.c
 * @brief MPI implementation of the trapezoidal rule for numerical integration.
 * 
 * @details Uses multiple processes to compute the integral of a function over a specified interval.
 * 
 * @note 
 * - Compile with an MPI compiler (e.g., `mpicc`).
 * - Run across multiple processes with `mpirun` or `mpiexec`.
 * 
 * Example usage:
 * @code
 * mpicc -o riemann_mpi riemann_mpi.c -lm
 * mpirun -np 4 ./riemann_mpi <limit a> <limit b>
 * @endcode
 * 
 * @authors
 * - Ramirez R. Andrea
 * 
 * @date 
 * - October 2024
 * 
 * @see
 * - MPI documentation: https://www.mpi-forum.org/docs/
 * - MPI programming guide: https://curc.readthedocs.io/en/latest/programming/MPI-C.html
 * 
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Global variable to hold the number of trapezoids
int N = 100000000; 

// Function to compute x^2
double f(double x) {
    return x * x; // Returns x squared
}

// Function to compute 2x^3
double g(double x) {
    return 2 * x * x * x; // Returns 2 times x cubed
}

// Function to compute sin(x)
double h_(double x) {
    return sin(x); // Returns the sine of x
}

// Function to compute the trapezoidal sum
double trapezoidal_sum(double param_a, double param_b, int local_n, double H) {
    double sum = 0.5 * (f(param_a) + f(param_b)); // Initial sum with endpoints

    // Sum the areas of the trapezoids
    for (int i = 1; i < local_n; i++) {
        double x = param_a + i * H; // Calculate x position
        sum += f(x); // Add f(x) to the sum
    }

    return sum * H;; // Return the total area
}

int main(int argc, char* argv[]) {
    // Variables to store the rank of the process and the total number of processes
    int PROCESS_RANK, CLUSTER_SIZE;
    double PARAM_A, PARAM_B, LOCAL_A, LOCAL_B;
    double LOCAL_SUM, GLOBAL_SUM;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the total number of processes in the MPI communicator
    MPI_Comm_size(MPI_COMM_WORLD, &CLUSTER_SIZE);

    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &PROCESS_RANK);

    if (PROCESS_RANK == 0) {
        if (argc < 3) {  // Check for at least 2 arguments
            printf("\nNot enough arguments passed: %d\n", argc);
            printf("Usage: %s <limit a> <limit b>\n", argv[0]);
            MPI_Finalize();
            return 1; 
        }

        // Validate the first argument (PARAM_A)
        char* endptr;
        PARAM_A = strtod(argv[1], &endptr);
        if (*endptr != '\0') {
            printf("Invalid first argument: %s. Please enter a number.\n", argv[1]);
            MPI_Finalize();
            return 1;
        }

        // Validate the second argument (PARAM_B)
        PARAM_B = strtod(argv[2], &endptr);
        if (*endptr != '\0') {
            printf("Invalid second argument: %s. Please enter a number.\n", argv[2]);
            MPI_Finalize();
            return 1;
        }
    }

    // Broadcast PARAM_A and PARAM_B from rank 0 to all processes
    MPI_Bcast(&PARAM_A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&PARAM_B, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate width H based on the number of trapezoids
    double H = (PARAM_B - PARAM_A) / N;
    
    // Determine the number of trapezoids each process will handle
    int LOCAL_N = N / CLUSTER_SIZE; // Base number of trapezoids
    if (PROCESS_RANK < N % CLUSTER_SIZE) {
        LOCAL_N++; // Distribute remainder
    }

    // Calculate the local interval bounds
    LOCAL_A = PARAM_A + PROCESS_RANK * (N / CLUSTER_SIZE) * H + (PROCESS_RANK < N % CLUSTER_SIZE ? PROCESS_RANK * H : (N % CLUSTER_SIZE) * H);
    LOCAL_B = LOCAL_A + LOCAL_N * H;

    // Start timing
    double START_TIME = MPI_Wtime();

    // Calculate the local trapezoidal sum
    LOCAL_SUM = trapezoidal_sum(LOCAL_A, LOCAL_B, LOCAL_N, H);

    if (PROCESS_RANK != 0) {
        // Send LOCAL_SUM to the master process
        MPI_Send(&LOCAL_SUM, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        // Master process receives LOCAL_SUM from each process
        GLOBAL_SUM = LOCAL_SUM; // Start with its own local sum
        double received_sum;
        for (int i = 1; i < CLUSTER_SIZE; i++) {
            MPI_Recv(&received_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            GLOBAL_SUM += received_sum; // Accumulate the received sums
        }

        // End timing
        double END_TIME = MPI_Wtime();

        // Display the final result
        printf("With n = %d, the approximation of the integral from a = %.2f to b = %.2f is %.6f\n", 
                N, PARAM_A, PARAM_B, GLOBAL_SUM);

        printf("Execution time: %.6f seconds\n", END_TIME - START_TIME);
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}