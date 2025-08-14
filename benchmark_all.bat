@echo off
REM benchmark_all.bat — Run benchmarks for all days

for /L %%d in (1,1,25) do (
    echo Running benchmarks for Day %%d...
    call benchmark.bat day%%d inputs\day%%d\input.txt
)
