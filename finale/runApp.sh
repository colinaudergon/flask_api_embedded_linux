#!/bin/bash

pathToPythonScript="app.py"
# Install Python dependencies
# Add your command for installing Python dependencies here
python3 -m pip install --upgrade Pillow
python3 -m pip install Flask
python3 -m pip install Flask-SocketIO
python3 -m pip install numpy
# Run Python program
python3 $pathToPythonScript
