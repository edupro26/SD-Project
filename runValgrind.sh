# Run Valgrind for all test executables

    echo "\nRunning Valgrind..."

    echo "\nTesting data:"
    valgrind --leak-check=yes ./binary/test_data

    echo "\nTesting entry:"
    valgrind --leak-check=yes ./binary/test_entry

    echo "\nTesting list:"
    valgrind --leak-check=yes ./binary/test_list

    echo "\nTesting table:"
    valgrind --leak-check=yes ./binary/test_table

    echo "\nTesting serialization:"
    valgrind --leak-check=yes ./binary/test_serialization

