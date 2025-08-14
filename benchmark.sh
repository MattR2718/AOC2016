#!/usr/bin/env bash
# benchmark.sh — Run hyperfine with high accuracy for a given executable and input file

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <executable_name> <input_file>"
    exit 1
fi

EXE="$1"
INPUT="$2"

# Verify that executable exists
if [ ! -x "build/$EXE/$EXE" ]; then
    echo "Error: 'build/$EXE/$EXE' not found or not executable."
    exit 1
fi

# Verify that input file exists
if [ ! -f "$INPUT" ]; then
    echo "Error: Input file '$INPUT' does not exist."
    exit 1
fi

# Ensure output directory exists
mkdir -p benchmarks

# Run hyperfine with warmup, pinned CPU, and export results
hyperfine \
    --warmup 3 \
    --runs 20 \
    --export-json "benchmarks/benchmark_${EXE}.json" \
    --shell "bash" \
    "./build/$EXE/$EXE < $INPUT"
