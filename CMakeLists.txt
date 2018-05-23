cmake_minimum_required (VERSION 2.8.11)
project(libnjones)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

link_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory (src)
add_subdirectory (test)

add_custom_target(
    format
    COMMAND cd ${CMAKE_SOURCE_DIR} && clang-format -sort-includes -i -style=file `find ./src -type f -name '*.cpp' -o -name '*.hpp'`
    COMMAND cd ${CMAKE_SOURCE_DIR} && clang-format -sort-includes -i -style=file `find ./test -type f -name '*.cpp' -o -name '*.hpp'`
)
