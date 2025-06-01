@echo off
setlocal enableextensions

REM
if not exist writers_logs (
    mkdir logs_writers
)
if not exist readers_logs (
    mkdir logs_readers
)

echo Starting writers...
for /L %%i in (0,1,49) do (
    start "" /B writer.exe %%i
)

REM 
timeout /t 2 >nul

echo Starting readers...
for /L %%i in (0,1,4) do (
    start "" /B reader.exe %%i
)

echo All processes launched.
endlocal
