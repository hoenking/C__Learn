cmake_minimum_required(VERSION 3.15)
project(C__Learn)

set(CMAKE_CXX_STANDARD 14)

add_executable(C__Learn main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(C__Learn LINK_PUBLIC Threads::Threads sqlite3)
