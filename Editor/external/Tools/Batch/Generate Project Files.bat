@echo off
if "%1"=="" (
	echo Source path not provided
	exit /b
)

if "%2"=="" (
	echo Project path not provided
	exit /b
)

cmake --target GameProject -DSF_PROJECT_PATH="%2" -s "%1"