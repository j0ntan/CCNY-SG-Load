add_subdirectory(thirdparty/googletest)
add_subdirectory(utils)

set(TESTS_DIR ${CMAKE_HOME_DIRECTORY}/tests)

add_executable(testing_Collect Collect.cpp)
target_include_directories(testing_Collect PRIVATE ${SG_LOAD_INCLUDE_DIR} ${TESTS_DIR})
target_link_libraries(testing_Collect
        InputSequence
        CollectPCSequence
        CollectdSPACESequence
        testing_utils_InputSequence
        gmock_main)

add_executable(testing_CollectKeypadSequence CollectKeypadSequence.cpp)
target_include_directories(testing_CollectKeypadSequence PRIVATE ${SG_LOAD_INCLUDE_DIR} ${TESTS_DIR})
target_link_libraries(testing_CollectKeypadSequence
        testing_globals
        testing_utils_InputSequence
        InputSequence
        CollectKeypadSequence
        gmock_main)

add_executable(testing_Encode Encode.cpp)
target_include_directories(testing_Encode PRIVATE
        ${SG_LOAD_INCLUDE_DIR}
        ${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/src)
target_link_libraries(testing_Encode Encode gmock_main)

add_executable(testing_InputSequence InputSequence.cpp)
target_include_directories(testing_InputSequence PRIVATE ${SG_LOAD_INCLUDE_DIR})
target_link_libraries(testing_InputSequence InputSequence gmock_main)

add_executable(testing_LoadProfile LoadProfile.cpp)
target_include_directories(testing_LoadProfile PRIVATE ${SG_LOAD_INCLUDE_DIR} ${TESTS_DIR})
target_link_libraries(testing_LoadProfile
        LoadProfile
        testing_utils_InputSequence
        InputSequence
        gmock_main)

add_executable(testing_RelayStateGenerator RelayStateGenerator.cpp)
target_include_directories(testing_RelayStateGenerator PRIVATE ${SG_LOAD_INCLUDE_DIR} ${TESTS_DIR})
target_link_libraries(testing_RelayStateGenerator
        RelayStateGenerator
        testing_utils_InputSequence
        gtest_main)

add_executable(ALL_tests
        Collect.cpp
        Encode.cpp
        InputSequence.cpp
        LoadProfile.cpp
        RelayStateGenerator.cpp)
target_include_directories(ALL_tests PRIVATE
        ${SG_LOAD_INCLUDE_DIR}
        ${TESTS_DIR}
        ${CMAKE_HOME_DIRECTORY}/CCNY_Smart_Grid_Load/src)
target_link_libraries(ALL_tests
        InputSequence
        CollectKeypadSequence
        CollectPCSequence
        CollectdSPACESequence
        LoadProfile
        testing_utils_InputSequence
        RelayStateGenerator
        Encode
        gmock_main)
