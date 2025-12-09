cmake_minimum_required(VERSION 3.10)

project(rocko)

find_package(nlohmann_json REQUIRED)

add_executable(rocko main.cpp)
target_link_libraries(rocko PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS rocko RUNTIME DESTINATION bin)
