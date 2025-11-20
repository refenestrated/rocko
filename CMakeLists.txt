cmake_minimum_required(VERSION 3.10)

project(rocko)

add_executable(rocko main.cpp)

install(TARGETS rocko RUNTIME DESTINATION bin)
