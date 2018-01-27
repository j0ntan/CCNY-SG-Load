add_subdirectory(lib/googlemock)
target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
        "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

include_directories(${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/include ./)

add_executable(COLLECT_tests Collect_tests.cpp)
target_link_libraries(COLLECT_tests gmock_main)
