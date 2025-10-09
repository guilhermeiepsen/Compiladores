#!/bin/bash

# Script to test the parser and view the AST with xdot
# Usage: ./test_and_view.sh <input_file>

if [ $# -eq 0 ]; then
    echo "Usage: $0 <input_file>"
    echo "Example: $0 tests/valid/ok_cabecalho_notype.txt"
    exit 1
fi

INPUT_FILE="$1"
OUTPUT_FILE="ast_output.dot"

echo "Parsing $INPUT_FILE..."
./calc < "$INPUT_FILE" > "$OUTPUT_FILE"

if [ $? -eq 0 ]; then
    echo "Parsing successful! Generated $OUTPUT_FILE"
    echo "Opening with xdot..."
    xdot "$OUTPUT_FILE" &
    echo "xdot opened in background. Close the window when done."
else
    echo "Parsing failed!"
    exit 1
fi
