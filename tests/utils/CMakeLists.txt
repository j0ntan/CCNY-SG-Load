add_library(testing_globals STATIC globals.cpp)
target_include_directories(testing_globals PRIVATE ${SG_LOAD_INCLUDE_DIR})

add_library(testing_utils_InputSequence STATIC InputSequence.cpp)
target_include_directories(testing_utils_InputSequence PRIVATE ${SG_LOAD_INCLUDE_DIR})
