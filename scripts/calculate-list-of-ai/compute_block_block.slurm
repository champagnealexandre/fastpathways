#!/bin/bash
#SBATCH --job-name=ai_calc_block100
#SBATCH --array=1-1000
#SBATCH --cpus-per-task=1
#SBATCH --mem=1G
#SBATCH --time=24:00:00
#SBATCH --output=logs2/ai_%A_%a.out
#SBATCH --error=logs2/ai_%A_%a.err

#SBATCH --partition=general
#SBATCH --qos=public

# This script is used to calculate the assembly index and time for a range of integers using the intchains program.
# The script is designed to be run on a SLURM cluster and uses an array job to parallelize the calculations.
# The script takes a block size of 100 integers and calculates the assembly index and time for each integer in the block.
# The results are saved in a CSV file for each block, with the integer, assembly index, and calculation time.

# The following line allows launching the job from another job (e.g., vscode)
unset $(printenv | grep SLURM_CPU_BIND | sed -E 's/(.*)=.*/\1/' | xargs)

# Calculate start and end of this block
BLOCK_SIZE=100
START=$(( ($SLURM_ARRAY_TASK_ID - 1) * BLOCK_SIZE + 1 ))
END=$(( $SLURM_ARRAY_TASK_ID * BLOCK_SIZE ))

OUTFILE="results2/block_${SLURM_ARRAY_TASK_ID}.csv"
echo "integer,assemblyindex,calc_time" > "$OUTFILE"

for ((i=START; i<=END; i++)); do
    OUT=$(./build/intchains time "$i" 2>/dev/null)
    if [ $? -ne 0 ] || [ -z "$OUT" ]; then
        echo "$i,ERROR,ERROR" >> "$OUTFILE"
        continue
    fi
    A=$(echo "$OUT" | sed -n '1p')
    T=$(echo "$OUT" | sed -n '2p' | sed 's/s//')
    echo "$i,$A,$T" >> "$OUTFILE"
done