#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/**
 * @file riemann_omp2.c
 * @brief This program calculates the approximate integral of a given function
 *        over a specified interval using the trapezoidal rule with OpenMP for parallelization.
 * @author Andrea Ximena Ramirez Recinos
 * @date Aug 11
 */

// Global variable to hold the number of trapezoids
int N = 100000000; 

// Global variable to hold the trapezoidal sum result
double sum_result = 0.0; 

// Function to compute x^2
// This function will be integrated over the specified interval
double f(double x) {
    return x * x; // Returns x squared
}

// Function to compute 2x^3
// This function will be integrated over the specified interval
double g(double x) {
    return 2 * x * x * x; // Returns 2 times x cubed
}

// Function to compute sin(x)
// This function will be integrated over the specified interval
double h_(double x) {
    return sin(x); // Returns the sine of x
}

// Function to compute the trapezoidal sum
void trapezoidal_sum(double param_a, double param_b, int local_n) {
    double h = (param_b - param_a) / N; // Width of each trapezoid
    double local_sum = 0.0; // Initialize local sum for each thread

    // Calculate the start and end points for each thread's segment
    double local_a = param_a + (omp_get_thread_num() * local_n * h);
    double local_b = local_a + (local_n * h);
    
    // Adjust local_b if it exceeds param_b
    if (local_b > param_b) {
        local_b = param_b;
    }

    // Start the local sum with the endpoints
    local_sum += 0.5 * (f(local_a) + f(local_b)); // Initial sum with endpoints

    // Sum the areas of the trapezoids for this segment
    for (int i = 1; i < local_n; i++) {
        double x = local_a + i * h; // Calculate x position
        local_sum += f(x); // Add h(x) to the sum
    }

    local_sum *= h; // Multiply by the width to get the total area for this segment

    // Use critical section to safely add to the global sum_result
    #pragma omp critical
    {
        sum_result += local_sum; // Accumulate the result
    }
}

int main(int argc, char* argv[]) {
    // Check for the required number of arguments
    if (argc < 4) {  
        printf("\nNot enough arguments passed: %d\n", argc);
        printf("Usage: %s <limit a> <limit b> <threads>\n", argv[0]);
        return 1; 
    }

    // Validate the first argument (PARAM_A)
    char* endptr;
    double PARAM_A = strtod(argv[1], &endptr);
    if (*endptr != '\0') {
        printf("Invalid first argument: %s. Please enter a number.\n", argv[1]);
        return 1;
    }

    // Validate the second argument (PARAM_B)
    double PARAM_B = strtod(argv[2], &endptr);
    if (*endptr != '\0') {
        printf("Invalid second argument: %s. Please enter a number.\n", argv[2]);
        return 1;
    }

    // Validate the third argument (threads)
    int OMP_NUM_THREADS = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') {
        printf("Invalid third argument: %s. Please enter a number.\n", argv[3]);
        return 1;
    }
    if ((N % OMP_NUM_THREADS) != 0) {
        printf("The number of trapezoids (N) must be divisible by the number of threads.\n");
        return 1;
    }

    // Set the number of threads for OpenMP
    omp_set_num_threads(OMP_NUM_THREADS);

    // Start timing
    double START_TIME = omp_get_wtime();

    int LOCAL_N = N / OMP_NUM_THREADS; // Number of trapezoids for each thread

    // Start the parallel region
    #pragma omp parallel
    {
        // Calculate the trapezoidal sum for the given limits
        trapezoidal_sum(PARAM_A, PARAM_B, LOCAL_N);
    }

    // Start timing
    double END_TIME = omp_get_wtime();

    // Print the result of the integral approximation
    printf("With n = %d, the approximation of the integral from point a = %.2f to point b = %.2f is %.6f\n", 
            N, PARAM_A, PARAM_B, sum_result);

    printf("Execution time: %.6f seconds\n", END_TIME - START_TIME);

    // Return success
    return 0; 
}