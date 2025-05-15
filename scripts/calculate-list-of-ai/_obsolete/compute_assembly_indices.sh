#!/bin/bash

# This script computes the assembly index for a range of integers from 1 to a specified maximum integer.
# It uses the `intchains` program to perform the calculations and stores the results in a CSV file.
# The script takes one argument: the maximum integer to compute the assembly index for.
# The output CSV file will contain three columns: integer, assembly index, and calculation time.
# Usage: ./compute_assembly_indices.sh <max_integer>

# Ensure an argument was provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <max_integer>"
    exit 1
fi

MAX_INT=$1
OUTPUT_FILE="assembly_indices.csv"
TMP_DIR=$(mktemp -d)

# Function to compute for one integer
compute_one() {
    i=$1
    OUT=$(./build/intchains time "$i")
    ASSEMBLY_INDEX=$(echo "$OUT" | sed -n '1p')
    TIME=$(echo "$OUT" | sed -n '2p' | sed 's/s//')
    echo "$i,$ASSEMBLY_INDEX,$TIME" > "$TMP_DIR/$i.csv"
}

export -f compute_one
export TMP_DIR

# Header
echo "integer,assemblyindex,calc_time" > "$OUTPUT_FILE"

# Generate list of integers, show progress, run in parallel (4 jobs default)
# seq 1 "$MAX_INT" | pv -pte -s "$MAX_INT" | parallel -j4 compute_one
# parallel --citation > /dev/null
seq 1 "$MAX_INT" | parallel -j$(nproc) compute_one  # Uses all available cores

# Concatenate all results in order
cat $(for i in $(seq 1 "$MAX_INT"); do echo "$TMP_DIR/$i.csv"; done) >> "$OUTPUT_FILE"

# Clean up
rm -r "$TMP_DIR"

echo "Done. Results written to $OUTPUT_FILE"