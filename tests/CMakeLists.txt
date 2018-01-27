add_subdirectory(lib/googlemock)
target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
        "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
