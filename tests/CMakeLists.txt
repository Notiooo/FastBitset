cmake_minimum_required(VERSION 3.14)
enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(FastBitset_test test_fastbitset.cpp)
target_link_libraries(FastBitset_test
        PRIVATE
        GTest::gtest_main
        FastBitset
)

include(GoogleTest)
gtest_discover_tests(FastBitset_test)
