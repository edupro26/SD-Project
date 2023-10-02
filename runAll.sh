# Run all test files on ./binary 
# Usage: ./runAll.sh

# Run all test files
    echo "\nRunning tests..."

    echo "\nTesting data:"
    ./binary/test_data

    echo "\nTesting entry:"
    ./binary/test_entry

    echo "\nTesting list:"
    ./binary/test_list

    echo "\nTesting table:"
    ./binary/test_table

    echo "\nTesting serialization:"
    ./binary/test_serialization
