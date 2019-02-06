cmake_minimum_required(VERSION 3.8)
project("SG-Load" CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SG_LOAD_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/include)

add_subdirectory(CCNY_Smart_Grid_Load)
add_subdirectory(tests)
