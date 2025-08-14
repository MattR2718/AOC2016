@echo off
REM run_all_days.bat — Run all days with timing using PowerShell Measure-Command

for /L %%d in (1,1,25) do (
    echo -----------------------------------------------------------
    echo Running day %%d

    powershell -Command "Measure-Command { Get-Content 'C:/Users/matth/source/repos/AOC2016/inputs/day%%d/input.txt' | & .\build\day%%d\Release\day%%d.exe | Out-Default }"

    echo -----------------------------------------------------------
)
