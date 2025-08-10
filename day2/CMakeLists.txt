cmake_minimum_required(VERSION 3.20)

add_executable(day2 day2.cpp)
target_link_libraries(day2 PRIVATE ctre mdspan)