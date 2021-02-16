cmake_minimum_required(VERSION 3.16.3)
project(MyProject VERSION 1.0.0)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -std=gnu17 -s -O2")

add_executable(solve_maze solve_maze.c bmp.c helper.c maze.c array_list.c)

target_link_libraries(solve_maze PRIVATE m)