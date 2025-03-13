cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(FastBitset_benchmark benchmark_fastbitset.cpp)
target_link_libraries(FastBitset_benchmark
        PRIVATE
        benchmark::benchmark
        FastBitset
)
