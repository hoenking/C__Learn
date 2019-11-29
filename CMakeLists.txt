cmake_minimum_required(VERSION 3.15)
project(C__Learn)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread
)
add_compile_options(-l sqlite3)
add_executable(C__Learn main.cpp sqlite3.c)
find_package(Threads REQUIRED)
target_link_libraries(C__Learn Threads::Threads)
