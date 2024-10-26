#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * @file riemann.c
 * @brief This program calculates the approximate integral of a given function
 *        over a specified interval using the trapezoidal rule.
 * @author Andrea Ximena Ramirez Recinos
 * @date Aug 11
 */

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
double trapezoidal_sum(double param_a, double param_b) {
    double h = (param_b - param_a) / N; // Width of each trapezoid
    double sum = 0.5 * (f(param_a) + f(param_b)); // Initial sum with endpoints

    // Sum the areas of the trapezoids
    for (int i = 1; i < N; i++) {
        double x = param_a + i * h; // Calculate x position
        sum += f(x); // Add f(x) to the sum
    }

    sum *= h; // Multiply by the width to get the total area
    return sum; // Return the total area
}

int main(int argc, char* argv[]) {
    if (argc < 3) {  // Check for at least 2 arguments
        printf("\nNot enough arguments passed: %d\n", argc);
        printf("Usage: %s <limit a> <limit b>\n", argv[0]);
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

    // Start timing
    clock_t START_TIME = clock();

    // Calculate the trapezoidal sum for the given limits
    double sum_result = trapezoidal_sum(PARAM_A, PARAM_B);


    // End timing
    clock_t END_TIME = clock();
    double elapsed_time = (double)(END_TIME - START_TIME) / CLOCKS_PER_SEC;

    // Print the result
    printf("With n = %d, the approximation of the integral from point a = %.2f to point b = %.2f is %.6f\n", 
            N, PARAM_A, PARAM_B, sum_result);

    printf("Execution time: %.6f seconds\n", elapsed_time);

    // Return success
    return 0; 
}