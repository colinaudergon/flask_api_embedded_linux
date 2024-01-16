#!/bin/bash
# Replace absolute path with the true application location 
#include dependency
# cd Documents/flask_api_embedded_linux
# current_dir=$(pwd)
# echo "Current directory: $current_dir"

pathToPythonScript="app.py"
pathToInputReading="inputReading.c"
pathToFrameBufferHandler="frameBufferHandler.c"

# Compile C program
gcc $pathToInputReading -o inputReading
if [ $? -ne 0 ]; then
    echo "Error: Compilation of C program failed."
    exit 1
else
    echo "C program compiled successfully."
fi

# Compile C frameBufferHandler programm
gcc $pathToFrameBufferHandler -o frameBufferHandler
if [ $? -ne 0 ]; then
    echo "Error: Compilation of frameBufferHandler program failed."
    exit 1
else
    echo "frameBufferHandler program compiled successfully."
fi

# Install Python dependencies
# Add your command for installing Python dependencies here

# Run Python program
python $pathToPythonScript
