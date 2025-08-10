#!/usr/bin/env bash

# benchmark_all.sh — Run benchmarks for all days

for day in {1..25}; do
    echo "Running benchmarks for Day $day..."
    ./benchmark.sh "day$day" "inputs/day$day/input.txt"
done
