@echo off
start "" "%~dp0LoLType.exe"

:keepRunning
timeout /t 30 /nobreak >nul
goto keepRunning
