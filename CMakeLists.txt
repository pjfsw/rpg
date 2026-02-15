cmake_minimum_required(VERSION 3.20)
project(rpg C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_executable(rpg
    src/main.c
    src/input/input.c
    src/screen/screen.c
    src/game/game.c)

target_include_directories(rpg PRIVATE include)
target_link_libraries(rpg PRIVATE SDL3::SDL3)