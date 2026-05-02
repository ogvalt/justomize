# run_test.cmake  – called by CTest for each integration test
#
# Variables injected by add_test():
#   justomize   – path to the built binary
#   base        – path to base JSON file
#   overlay     – path to overlay JSON file
#   expected    – path to file containing expected stdout (empty = don't check)
#   exit_code   – expected exit code (0 or non-zero)

execute_process(
    COMMAND "${justomize}" "${base}" "${overlay}"
    OUTPUT_VARIABLE actual_stdout
    ERROR_VARIABLE  actual_stderr
    RESULT_VARIABLE actual_exit
)

# Check exit code
if(exit_code EQUAL 0)
    if(NOT actual_exit EQUAL 0)
        message(FATAL_ERROR
            "Expected exit 0 but got ${actual_exit}\nstderr: ${actual_stderr}")
    endif()
else()
    if(actual_exit EQUAL 0)
        message(FATAL_ERROR "Expected non-zero exit but got 0")
    endif()
    # Non-zero exit: nothing more to check
    return()
endif()

# Check stdout when expected file is non-empty
file(READ "${expected}" expected_content)
if(NOT expected_content STREQUAL "")
    if(NOT actual_stdout STREQUAL expected_content)
        message(FATAL_ERROR
            "Output mismatch.\n--- expected ---\n${expected_content}\n--- actual ---\n${actual_stdout}")
    endif()
endif()
