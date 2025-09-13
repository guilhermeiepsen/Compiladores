#!/bin/bash

# Script to test the syntax analyzer with entrada2.txt

# Define test directories
TEST_DIR="tests"
VALID_DIR="$TEST_DIR/valid"
INVALID_DIR="$TEST_DIR/invalid"

# Check if the parser executable exists
if [ ! -f "etapa2" ]; then
    echo "Error: Parser executable 'etapa2' not found!"
    exit 1
fi

# Function to run tests and log errors to terminal
run_tests() {
    local test_type=$1
    local dir=$2
    echo "Running $test_type tests..."
    for test_file in $dir/*.txt; do
        echo "Testing $test_file..."
        ./etapa2 < "$test_file" 2>&1 | tee /dev/tty
    done
}

# Run valid and invalid tests
run_tests "valid" "$VALID_DIR"
run_tests "invalid" "$INVALID_DIR"