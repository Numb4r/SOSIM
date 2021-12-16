cmake_minimum_required(VERSION 3.22.1)

project(SOSIM)

add_executable(${PROJECT_NAME} main.cpp)


add_subdirectory(shell)
add_subdirectory(core/hardware)

