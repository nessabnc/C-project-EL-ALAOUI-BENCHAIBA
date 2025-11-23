cmake_minimum_required(VERSION 3.6)
project(TI_301_PRJ_STUDENTS-master C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

include_directories(include)

add_executable(markov
        main.c
        src/adjlist.c
        src/tarjan.c
        src/hasse.c
        src/matrix.c
)
