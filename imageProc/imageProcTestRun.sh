#!/bin/bash
# Replace absolute path with the true application location
# Include dependency
cd ..
current_dir=$(pwd)
echo "Current directory: $current_dir"

cmd="git pull"
echo $cmd
git stash # Stash local changes before pulling
git pull
git stash pop # Apply stashed changes

cd "imageProc"

pathToPythonScript="imageProc.py"
pathToCprogram=$(realpath mainWithArg.c)  # Full path to the C program

# Compile C program
gcc $pathToCprogram -o frameBufferHandler
if [ $? -ne 0 ]; then
    echo "Error: Compilation of C program failed."
    exit 1
else
    echo "C program compiled successfully."
fi

# Install Python dependencies
# Add your command for installing Python dependencies here

# Run Python script
echo "Running python script."
python3 $pathToPythonScript  # Use python3 for Python 3

# Give execution rights to the script
chmod +x imageProcTestRun.sh