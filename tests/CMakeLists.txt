add_subdirectory(lib/googlemock)
target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
        "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

include_directories(${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/include ./)
