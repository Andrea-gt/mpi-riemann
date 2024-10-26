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

# Create the output CSV file if it doesn't exist, and append the header if it's new
OUTPUT_FILE="execution_times.csv"
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "Program,Execution Time (seconds)" > "$OUTPUT_FILE"
fi

echo "F(x) = (x)^2, N = 100,000,000; "

# Run the non-MPI Riemann program and capture execution time
echo "Running riemann.c..."
./riemann "$PARAM_A" "$PARAM_B" &> output_riemann.txt &
PID1=$!

# Run the OpenMP Riemann program and capture execution time
echo "Running riemann_omp.c..."
./riemann_omp "$PARAM_A" "$PARAM_B" "$THREADS" &> output_riemann_omp.txt &
PID2=$!

# Run the MPI Riemann program and capture execution time
echo "Running riemann_mpi.c..."
mpirun -np 4 ./riemann_mpi "$PARAM_A" "$PARAM_B" &> output_riemann_mpi.txt &
PID3=$!

# Wait for all background processes to complete
wait $PID1
wait $PID2
wait $PID3

# Extract execution times from output files and append to CSV
for OUTPUT in output_riemann.txt output_riemann_omp.txt output_riemann_mpi.txt; do
    PROGRAM_NAME=$(basename "$OUTPUT" .txt)
    EXEC_TIME=$(grep -oP "Execution time: \K[\d.]+(?= seconds)" "$OUTPUT")  # Using regex to extract the float value
    if [ -z "$EXEC_TIME" ]; then
        EXEC_TIME="N/A"  # If the execution time isn't found, mark as N/A
    fi
    echo "$PROGRAM_NAME,$EXEC_TIME" >> "$OUTPUT_FILE"  # Append execution time to CSV
done

# Sort the CSV file by program name (ignoring the header) and overwrite it
{ head -n 1 "$OUTPUT_FILE"; tail -n +2 "$OUTPUT_FILE" | sort; } > temp_file && mv temp_file "$OUTPUT_FILE"

# Clean up compiled programs and output text files
rm riemann riemann_mpi riemann_omp output_riemann.txt output_riemann_omp.txt output_riemann_mpi.txt