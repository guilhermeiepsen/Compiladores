#!/bin/bash

# Script to test the syntax analyzer with entrada2.txt

# Check if the parser executable exists
if [ ! -f "etapa2" ]; then
    echo "Error: Parser executable 'etapa2' not found!"
    exit 1
fi

# Check if entrada2.txt exists
if [ ! -f "input2.txt" ]; then
    echo "Error: Input file 'input2.txt' not found!"
    exit 1
fi

# Run the parser with entrada2.txt and capture the output
echo "Running syntax analyzer on input2.txt..."
./etapa2 < input2.txt > syntax_output.txt 2>&1

# Display the output
echo "Syntax analysis completed. Output:"
cat syntax_output.txt

# Check for errors in the output
if grep -q "ERROR FOUND" syntax_output.txt; then
    echo "Syntax errors were found in the input."
else
    echo "No syntax errors found."
fi