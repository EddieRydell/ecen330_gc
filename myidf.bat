@echo off
REM Check if the first argument is provided
if "%1"=="" (
    echo Please provide a directory name.
    exit /b 1
)

REM Check if the environment variable ESP_EXPORT_DONE is set
if "%ESP_EXPORT_DONE%"=="1" (
    echo export.bat has already been run in this session.
) else (
    REM Change directory to esp/esp-idf
    cd ../../esp/esp-idf

    REM Execute export.bat
    call export.bat

    REM Set the environment variable to indicate export.bat has been run
    set ESP_EXPORT_DONE=1

    REM Change back to the original directory
    cd - 
)

REM Change directory to the specific CLionProjects directory
cd ../../CLionProjects/ecen330_gc/%1

REM Build the project
idf.py build

REM Flash the project
idf.py flash

REM Check if the second argument is -m
if "%2"=="-m" (
    idf.py monitor
)

REM Get back to the same directory we started in
cd ..