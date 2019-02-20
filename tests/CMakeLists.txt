add_subdirectory(thirdparty/googletest)

set(MOCKS_DIR ${CMAKE_HOME_DIRECTORY}/tests)

add_library(InputSequence_utils STATIC InputSequence_utils.cpp)
target_include_directories(InputSequence_utils PRIVATE ${SG_LOAD_INCLUDE_DIR})

add_executable(COLLECT_tests Collect_tests.cpp)
target_include_directories(COLLECT_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(COLLECT_tests
        InputSequence
        CollectKeypadSequence
        CollectPCSequence
        CollectdSPACESequence
        InputSequence_utils
        gmock_main)

add_executable(ENCODE_tests Encode_tests.cpp)
target_include_directories(ENCODE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(ENCODE_tests Encode gmock_main)

add_executable(INPUTSEQUENCE_tests InputSequence_tests.cpp)
target_include_directories(INPUTSEQUENCE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(INPUTSEQUENCE_tests InputSequence gmock_main)

add_executable(LOADPROFILE_tests LoadProfile_tests.cpp)
target_include_directories(LOADPROFILE_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(LOADPROFILE_tests
        LoadProfile
        InputSequence_utils
        InputSequence
        gmock_main)

add_executable(RELAYSTATEGENERATOR_tests RelayStateGenerator_tests.cpp)
target_include_directories(RELAYSTATEGENERATOR_tests PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(RELAYSTATEGENERATOR_tests
        RelayStateGenerator
        InputSequence_utils
        gtest_main)

add_executable(ALL_tests
        Collect_tests.cpp
        Encode_tests.cpp
        InputSequence_tests.cpp
        LoadProfile_tests.cpp
        RelayStateGenerator_tests.cpp)
target_include_directories(ALL_tests PRIVATE ${SG_LOAD_INCLUDE_DIR} ${MOCKS_DIR})
target_link_libraries(ALL_tests
        InputSequence
        CollectKeypadSequence
        CollectPCSequence
        CollectdSPACESequence
        LoadProfile
        InputSequence_utils
        RelayStateGenerator
        Encode
        gmock_main)
