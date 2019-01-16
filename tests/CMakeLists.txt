add_subdirectory(thirdparty/googletest)

set(SG_LOAD_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/include)
set(MOCKS_DIR ${CMAKE_HOME_DIRECTORY}/tests)

add_executable(COLLECT_tests Collect_tests.cpp)
target_include_directories(COLLECT_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(COLLECT_tests gmock_main)

add_executable(SCAN_tests Scan_tests.cpp)
target_include_directories(SCAN_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(SCAN_tests gtest_main)

add_executable(PARSE_tests Parse_tests.cpp)
target_include_directories(PARSE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(PARSE_tests gtest_main)

add_executable(ENCODE_tests Encode_tests.cpp)
target_include_directories(ENCODE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(ENCODE_tests gmock_main)

add_executable(INPUTSEQUENCE_tests
        InputSequence_tests.cpp
        ${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/src/InputSequence.cpp)
target_include_directories(INPUTSEQUENCE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(INPUTSEQUENCE_tests gmock_main)

#[[
# Below target not working due to multiple definition of helper functions
# from Scan.h.
add_executable(ALL_tests Collect_tests.cpp
        Scan_tests.cpp
        Parse_tests.cpp
        Encode_tests.cpp)
target_include_directories(ALL_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(ALL_tests PRIVATE gmock_main)
#]]