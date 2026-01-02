#!/bin/bash
# Build and run script for Minesweeper

# Build project and check if build was successful
if cmake --build build; then
    # Run the executable
    ./build/minesweeper
else
    echo "Build failed!"
    exit 1
fi

