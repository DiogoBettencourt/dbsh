cmake_minimum_required(VERSION 3.10)
project(dbsh)

set(CMAKE_CXX_STANDARD 17)

add_executable(dbsh main.cpp parser.cpp shell.cpp executor.cpp builtins.cpp)
add_executable(tests test_parser.cpp test_builtins.cpp test_executor.cpp parser.cpp builtins.cpp executor.cpp)

# Find and link Catch2 for tests
find_package(Catch2 REQUIRED)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
