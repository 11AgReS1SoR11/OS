cmake_minimum_required(VERSION 3.5)

project(Demon)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(test)

add_executable(Demon main.cpp)
target_link_libraries(Demon PRIVATE DemonLib)
