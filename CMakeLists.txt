cmake_minimum_required(VERSION 3.20)
project(rpg C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(rpg
    src/input/input.c
    src/screen/font.c
    src/screen/screen.c
    src/screen/fileutil.c
    src/game/game.c
    src/game/textbox.c
    src/game/terminal.c
    src/game/world.c
    src/main.c
    )

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_custom_command(TARGET rpg POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_DIR}
            $<TARGET_FILE_DIR:rpg>/assets
)

target_compile_definitions(rpg PRIVATE
    RPG_ASSETS_DIR="$<TARGET_FILE_DIR:rpg>/assets"
)    

add_custom_target(run
    COMMAND $<TARGET_FILE:rpg>
    DEPENDS rpg
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
)

add_library(inih STATIC
    external/inih/ini.c
)

target_include_directories(inih PUBLIC
    external/inih
)

target_include_directories(rpg PRIVATE include)
target_link_libraries(rpg PRIVATE SDL3::SDL3 SDL3_image::SDL3_image inih)
