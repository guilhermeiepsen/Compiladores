#!/bin/bash

# Script to run lexical analyzer tests
# Author: Bruno e Guilherme
# Usage: ./run_tests.sh

# Possible running commands:
# ./run_tests.sh                # Run tests normally
# ./run_tests.sh -s              # Show test input before running
# ./run_tests.sh --show-input    # Same as -s
# ./run_tests.sh -c              # Compare output with expected results
# ./run_tests.sh --compare       # Same as -c
# ./run_tests.sh --help          # Show help message

echo "=========================================="
echo "     Lexical Analyzer Test Runner"
echo "=========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if required files exist
check_files() {
    echo -e "${BLUE}Checking required files...${NC}"
    
    if [ ! -f "scanner.l" ]; then
        echo -e "${RED}Error: scanner.l not found!${NC}"
        exit 1
    fi
    
    if [ ! -f "main.c" ]; then
        echo -e "${RED}Error: main.c not found!${NC}"
        exit 1
    fi
    
    if [ ! -f "tokens.h" ]; then
        echo -e "${RED}Error: tokens.h not found!${NC}"
        exit 1
    fi
    
    if [ ! -f "makefile" ]; then
        echo -e "${RED}Error: makefile not found!${NC}"
        exit 1
    fi
    
    if [ ! -f "test_etapa1.txt" ]; then
        echo -e "${RED}Error: test_etapa1.txt not found!${NC}"
        exit 1
    fi
    
    if [ ! -f "expected_output.txt" ]; then
        echo -e "${YELLOW}Warning: expected_output.txt not found (needed for comparison mode)${NC}"
    fi
    
    echo -e "${GREEN}All required files found.${NC}"
}

# Build the project
build_project() {
    echo -e "\n${BLUE}Building project...${NC}"
    
    # Clean previous build
    make clean > /dev/null 2>&1
    
    # Build the project
    if make; then
        echo -e "${GREEN}Build successful!${NC}"
    else
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
}

# Run the lexical analyzer
run_analyzer() {
    echo -e "\n${BLUE}Running lexical analyzer on test_etapa1.txt...${NC}"
    echo "=========================================="
    
    if [ -f "etapa1" ]; then
        # Run the analyzer and capture the exit code
        ./etapa1 < test_etapa1.txt > actual_output.txt
        exit_code=$?
        
        echo "=========================================="
        
        case $exit_code in
            0)
                echo -e "${GREEN}Analysis completed successfully (no errors)${NC}"
                ;;
            1)
                echo -e "${YELLOW}Analysis stopped due to lexical error${NC}"
                ;;
            2)
                echo -e "${RED}Analysis failed - unknown token encountered${NC}"
                ;;
            *)
                echo -e "${RED}Analysis failed with exit code: $exit_code${NC}"
                ;;
        esac
    else
        echo -e "${RED}Error: etapa1 executable not found!${NC}"
        exit 1
    fi
}

# Run analyzer with comparison to expected output
run_analyzer_with_comparison() {
    echo -e "\n${BLUE}Running lexical analyzer with output comparison...${NC}"
    echo "=========================================="
    
    if [ ! -f "expected_output.txt" ]; then
        echo -e "${RED}Error: expected_output.txt not found for comparison!${NC}"
        exit 1
    fi
    
    if [ -f "etapa1" ]; then
        # Run the analyzer and save output to temporary file
        ./etapa1 < test_etapa1.txt > actual_output.tmp 2>&1
        exit_code=$?
        
        echo "=========================================="
        
        case $exit_code in
            0)
                echo -e "${GREEN}Analysis completed successfully${NC}"
                # Compare outputs
                if diff -q expected_output.txt actual_output.tmp > /dev/null; then
                    echo -e "${GREEN}✓ Output matches expected results perfectly!${NC}"
                else
                    echo -e "${YELLOW}⚠ Output differs from expected results${NC}"
                    echo -e "${BLUE}Differences:${NC}"
                    diff expected_output.txt actual_output.tmp || true
                fi
                ;;
            1)
                echo -e "${YELLOW}Analysis stopped due to lexical error${NC}"
                echo -e "${BLUE}Actual output:${NC}"
                cat actual_output.tmp
                ;;
            2)
                echo -e "${RED}Analysis failed - unknown token encountered${NC}"
                echo -e "${BLUE}Actual output:${NC}"
                cat actual_output.tmp
                ;;
            *)
                echo -e "${RED}Analysis failed with exit code: $exit_code${NC}"
                echo -e "${BLUE}Actual output:${NC}"
                cat actual_output.tmp
                ;;
        esac
        
        # Clean up temporary file
        rm -f actual_output.tmp
    else
        echo -e "${RED}Error: etapa1 executable not found!${NC}"
        exit 1
    fi
}

# Show test file content (optional)
show_test_content() {
    if [ "$1" = "--show-input" ] || [ "$1" = "-s" ]; then
        echo -e "\n${BLUE}Test file content:${NC}"
        echo "=========================================="
        cat -n test_etapa1.txt
        echo "=========================================="
    fi
}

# Main execution
main() {
    local compare_mode=""
    
    # Check for comparison mode
    if [ "$1" = "--compare" ] || [ "$1" = "-c" ]; then
        compare_mode="true"
    fi
    
    # Show input if requested
    show_test_content "$1"
    
    # Check files exist
    check_files
    
    # Build project
    build_project
    
    # Run analyzer (with or without comparison)
    if [ "$compare_mode" = "true" ]; then
        run_analyzer_with_comparison
    else
        run_analyzer
    fi
    
    echo -e "\n${GREEN}Test run completed!${NC}"
    if [ "$compare_mode" = "true" ]; then
        echo -e "${BLUE}Comparison mode was used to validate output.${NC}"
    else
        echo -e "${BLUE}For output comparison, run: ./run_tests.sh --compare${NC}"
    fi
    echo -e "${BLUE}To see the test input, run: ./run_tests.sh --show-input${NC}"
}

# Show help
show_help() {
    echo "Lexical Analyzer Test Runner"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help        Show this help message"
    echo "  -s, --show-input  Show test file content before running"
    echo "  -c, --compare     Compare output with expected results"
    echo ""
    echo "Examples:"
    echo "  $0                Run tests normally"
    echo "  $0 -s             Run tests and show input file"
    echo "  $0 -c             Run tests with output comparison"
    echo "  $0 --help         Show this help"
}

# Handle command line arguments
case "$1" in
    -h|--help)
        show_help
        exit 0
        ;;
    *)
        main "$1"
        ;;
esac
