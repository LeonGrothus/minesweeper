#!/bin/bash
# Build and run script for Minesweeper

# Build the project
cmake --build build

# Check if build was successful
if [ $? -eq 0 ]; then
    # Run the executable
    ./build/minesweeper
else
    echo "Build failed!"
    exit 1
fi

