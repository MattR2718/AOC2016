cmake_minimum_required(VERSION 3.20)

add_executable(day3 day3.cpp)
target_link_libraries(day3 PRIVATE ctre mdspan)