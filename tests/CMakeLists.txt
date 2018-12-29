add_subdirectory(thirdparty/googletest)

include_directories(${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/include ./)

add_executable(COLLECT_tests Collect_tests.cpp)
target_link_libraries(COLLECT_tests gmock_main)

add_executable(SCAN_tests Scan_tests.cpp)
target_link_libraries(SCAN_tests gtest_main)

add_executable(PARSE_tests Parse_tests.cpp)
target_link_libraries(PARSE_tests gtest_main)

add_executable(ENCODE_tests Encode_tests.cpp)
target_link_libraries(ENCODE_tests gmock_main)
