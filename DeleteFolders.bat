@echo off

echo batch location %~dp0

echo Removing ".vs"
RMDIR "%~dp0.vs" /S /Q

echo Removing "Binaries"
RMDIR "%~dp0Binaries" /S /Q

echo Removing "DerivedDataCache"
RMDIR "%~dp0DerivedDataCache" /S /Q

echo Removing "Intermediate"
RMDIR "%~dp0Intermediate" /S /Q

rem echo Removing "Saved"
rem RMDIR "Saved" /S /Q