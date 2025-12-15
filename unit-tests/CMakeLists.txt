cmake_minimum_required(VERSION 3.10)
project(unit_tests_verifyAddress LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Enable testing
include(CTest)
enable_testing()

# Coverage flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -pedantic -g -O0 --coverage")
set(GCC_COVERAGE_LINK_FLAGS "--coverage -lgcov")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# Include directories
include_directories(
    ../src
    ../src/common
    ../src/common/ui
    host_stubs
)

# Add test executable
#add_executable(test_verifyAddress test_verifyAddress.c ../src/verifyAddress.c)

# Link with CMocka and coverage
#target_link_libraries(test_verifyAddress PUBLIC cmocka gcov m)

# Register the test
#add_test(NAME test_verifyAddress COMMAND test_verifyAddress)
