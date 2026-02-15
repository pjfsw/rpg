cmake_minimum_required(VERSION 3.20)
project(rpg C)
set(CMAKE_C_STANDARD 11)

find_package(SDL3 REQUIRED)

add_executable(rpg src/main.c)

target_link_libraries(rpg PRIVATE SDL3::SDL3)