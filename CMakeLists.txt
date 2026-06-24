cmake_minimum_required(VERSION 3.16)
project(KastilTerkutuk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(game main.cpp)
target_link_libraries(game PRIVATE sfml-graphics sfml-window sfml-system)

# Salin folder assets ke direktori build
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game>/assets)