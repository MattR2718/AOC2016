@echo off
REM benchmark.bat — Run hyperfine with high accuracy for a given executable and input file

REM Check arguments
if "%~2"=="" (
    echo Usage: %~nx0 ^<executable_name^> ^<input_file^>
    exit /b 1
)

set EXE=%1
set INPUT=%2

REM Verify that executable exists
if not exist "build\%EXE%\Release\%EXE%.exe" (
    echo Error: build\%EXE%\Release\%EXE%.exe not found.
    exit /b 1
)

REM Verify that input file exists
if not exist "%INPUT%" (
    echo Error: Input file %INPUT% does not exist.
    exit /b 1
)

REM Ensure output directory exists
if not exist benchmarks (
    mkdir benchmarks
)

REM Run hyperfine
hyperfine ^
    --warmup 3 ^
    --prepare "build\%EXE%\%EXE%.exe < %INPUT%" ^
    --runs 20 ^
    --export-json "benchmarks\benchmark_%EXE%.json" ^
    --shell "cmd" ^
    "build\%EXE%\%EXE%.exe < %INPUT%"
