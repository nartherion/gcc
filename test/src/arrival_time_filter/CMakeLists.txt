project(nartherion-gcc-arrival-time-filter-tests)
add_executable(${PROJECT_NAME} packet_group_tracker_tests.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    nartherion-gcc-arrival-time-filter
    nartherion-gcc-test-utils)
gtest_discover_tests(${PROJECT_NAME})
