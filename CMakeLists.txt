cmake_minimum_required(VERSION 3.23)
project(cow-benchmarks)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Setup Catch2
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.0.1 # or a later release
)

# Setup fmt::format
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 9.1.0 # or a later release
)

FetchContent_MakeAvailable(Catch2 fmt)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

include_directories(${CMAKE_SOURCE_DIR} ${fmt_SOURCE_DIR}/include)

##### Define unit tests
add_executable(tests ${SOURCE_FILES} tests/test_algorithms.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain fmt::fmt)
target_compile_options(tests PRIVATE -fsanitize=address)
target_link_options(tests PUBLIC -fsanitize=address)
catch_discover_tests(tests)

##### Define interpreter
#add_executable(tungsten main.cpp ${SOURCE_FILES})