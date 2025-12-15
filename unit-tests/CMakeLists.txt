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


include_directories(
    ../src
    ../src/common
    ../src/common/ui
    ../ledger-secure-sdk/lib_cxng/include 
    ../ledger-secure-sdk/include 
    ../ledger-secure-sdk/lib_standard_app
    ../ledger-secure-sdk/target/nanos2/include 
    host_stubs
)

add_library(sdk_mocks STATIC
    host_stubs/sdk_mocks.c
)

target_include_directories(sdk_mocks PUBLIC
    ../src
    ../src/common
    ../ledger-secure-sdk/lib_cxng/include
    host_stubs
)
target_compile_definitions(sdk_mocks PUBLIC HAVE_ECC HAVE_BLS12_381_G1_CURVE)


add_executable(test_verifyAddress test_verifyAddress.c ../src/verifyAddress.c)
target_link_libraries(test_verifyAddress PRIVATE sdk_mocks cmocka gcov m)


# Теперь можно добавлять макросы, include и линковку
target_compile_definitions(test_verifyAddress PRIVATE HAVE_ECC)
target_compile_definitions(test_verifyAddress PRIVATE HAVE_HASH)
target_compile_definitions(test_verifyAddress PRIVATE HAVE_BLS12_381_G1_CURVE)
target_compile_definitions(test_verifyAddress PRIVATE HAVE_RSA)
target_compile_definitions(test_verifyAddress PRIVATE HAVE_ECC_WEIERSTRASS)
target_compile_definitions(test_verifyAddress PRIVATE HAVE_SHA256)


target_compile_options(test_verifyAddress PRIVATE --coverage -O0 -g)
target_link_libraries(test_verifyAddress PUBLIC sdk_mocks cmocka gcov m)

# Register the test
add_test(NAME test_verifyAddress COMMAND test_verifyAddress)


target_include_directories(sdk_mocks PUBLIC
    ../src
    ../src/common
    ../ledger-secure-sdk/lib_cxng/include
    host_stubs
)



