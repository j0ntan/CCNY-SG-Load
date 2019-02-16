add_subdirectory(thirdparty/googletest)

set(MOCKS_DIR ${CMAKE_HOME_DIRECTORY}/tests)

add_executable(COLLECT_tests Collect_tests.cpp)
target_include_directories(COLLECT_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(COLLECT_tests
        InputSequence
        CollectKeypadSequence
        CollectPCSequence
        CollectdSPACESequence
        gmock_main)

add_executable(SCAN_tests Scan_tests.cpp)
target_include_directories(SCAN_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(SCAN_tests InputSequence gtest_main)

add_executable(PARSE_tests Parse_tests.cpp)
target_include_directories(PARSE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(PARSE_tests InputSequence gtest_main)

add_executable(ENCODE_tests Encode_tests.cpp)
target_include_directories(ENCODE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(ENCODE_tests gmock_main)

add_executable(INPUTSEQUENCE_tests InputSequence_tests.cpp)
target_include_directories(INPUTSEQUENCE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(INPUTSEQUENCE_tests InputSequence gmock_main)

add_executable(LOADPROFILE_tests LoadProfile_tests.cpp)
target_include_directories(LOADPROFILE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(LOADPROFILE_tests InputSequence LoadProfile gmock_main)

add_executable(RELAYSTATEGENERATOR_tests RelayStateGenerator_tests.cpp)
target_include_directories(RELAYSTATEGENERATOR_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(RELAYSTATEGENERATOR_tests RelayStateGenerator gtest_main)

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
