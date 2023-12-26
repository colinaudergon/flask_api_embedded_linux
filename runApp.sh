#!/bin/bash
# Replace absolute path with the true application location 
cd Documents/flask_api_embedded_linux
current_dir=$(pwd)
echo "Current directory: $current_dir"

pathToPythonScript="app.py"
pathToCprogram="app.c"

# Compile C program
gcc $pathToCprogram -o app_executable
if [ $? -ne 0 ]; then
    echo "Error: Compilation of C program failed."
    exit 1
else
    echo "C program compiled successfully."
fi

# Install Python dependencies
# Add your command for installing Python dependencies here

# Run Python program
python $pathToPythonScript

# Run compiled C program
# ./app_executable
