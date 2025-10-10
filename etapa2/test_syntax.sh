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
    local correct_count=0
    local total_count=0
    echo "Running $test_type tests..."
    for test_file in $dir/*.txt; do
        echo "Testing $test_file..."
        if ./etapa2 < "$test_file" 2>&1 | grep -q "syntax error"; then
            if [ "$test_type" == "invalid" ]; then
                correct_count=$((correct_count + 1))
            fi
        else
            if [ "$test_type" == "valid" ]; then
                correct_count=$((correct_count + 1))
            fi
        fi
        total_count=$((total_count + 1))
    done
    echo -e "\033[1;34m$test_type tests: $correct_count out of $total_count correct\033[0m"
    if [ "$correct_count" -eq "$total_count" ]; then
        echo -e "\033[0;32mSuccess: All $test_type tests passed!\033[0m"
    else
        echo -e "\033[0;33mAlert: Some $test_type tests failed!\033[0m"
    fi
}

# Run valid and invalid tests
run_tests "valid" "$VALID_DIR"
run_tests "invalid" "$INVALID_DIR"