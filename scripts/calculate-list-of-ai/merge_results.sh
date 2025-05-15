#!/bin/bash

OUTPUT="assembly_indices.csv"

# Write header once
echo "integer,assemblyindex,calc_time" > "$OUTPUT"

# Merge all results, skipping header lines
tail -n +2 -q results/block_*.csv >> "$OUTPUT"
echo "Merged results written to $OUTPUT"