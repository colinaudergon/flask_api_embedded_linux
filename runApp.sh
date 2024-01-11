#!/bin/bash
# Replace absolute path with the true application location 
#include dependency
cd Documents/flask_api_embedded_linux
current_dir=$(pwd)
echo "Current directory: $current_dir"

pathToPythonScript="app.py"
pathToCprogram="app.c"
# Add correct path to screen proogramm
# pathToScreenProgram="app.c"

# Compile C program
gcc $pathToCprogram -o app_executable
if [ $? -ne 0 ]; then
    echo "Error: Compilation of C program failed."
    exit 1
else
    echo "C program compiled successfully."
fi
# Compile C frameBufferHandler programm
# gcc $pathToCprogram -o frameBufferHandler
# if [ $? -ne 0 ]; then
#     echo "Error: Compilation of frameBufferHandler program failed."
#     exit 1
# else
#     echo "frameBufferHandler program compiled successfully."
# fi

# Install Python dependencies
# Add your command for installing Python dependencies here

# Run Python program
python $pathToPythonScript
