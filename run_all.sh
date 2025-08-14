#!/usr/bin/env bash

for day in {1..25}
do
    echo "-----------------------------------------------------------"
    echo "Running day $day"
    time ./build/day$day/day$day < inputs/day$day/input.txt
    echo "-----------------------------------------------------------"

done