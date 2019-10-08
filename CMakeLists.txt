cmake_minimum_required(VERSION 3.15)
project(TestInterface)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS network graphics window system REQUIRED)

add_executable(Test main.cpp)
target_include_directories(Test PUBLIC ${SFML_INCLUDE_DIR})
target_link_libraries(Test sfml-system sfml-window sfml-graphics sfml-network)