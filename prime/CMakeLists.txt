cmake_minimum_required(VERSION 3.5 FATAL_ERROR) 

project(hi LANGUAGES C)

# add_compile_options(-W -Wall -lpthread -lm)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE) 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -lpthread -lm")

# list(APPEND flags "-o" "-g"  "-Wall")
# message("flags : ${flags}")

add_executable(hi hi.c)


