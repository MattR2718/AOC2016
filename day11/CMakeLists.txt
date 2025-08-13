cmake_minimum_required(VERSION 3.20)

add_executable(day11 day11.cpp)
target_link_libraries(day11 PRIVATE ctre Boost::bimap)