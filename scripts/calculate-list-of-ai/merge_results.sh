#!/bin/bash

# This script merges the results of assembly index calculations from multiple CSV files into a single CSV file.
# It assumes that the results are stored in files named "block_<number>.csv" in the "results" directory.
# The merged file will be named "assembly_indices.csv" and will contain three columns: integer, assembly index, and calculation time.
# Usage: ./merge_results.sh

OUTPUT="assembly_indices.csv"

# Write header
echo "integer,assemblyindex,calc_time" > "$OUTPUT"

# Merge, skip headers, and sort numerically by first column (integer)
tail -n +2 -q results/block_*.csv | sort -t, -k1,1n >> "$OUTPUT"

echo "Merged and sorted results written to $OUTPUT"