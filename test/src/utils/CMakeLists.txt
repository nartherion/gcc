project(nartherion-gcc-test-utils)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${NARTHERION_GCC_SOURCE_DIR}/test/include)
target_link_libraries(${PROJECT_NAME} INTERFACE
    nartherion-gcc-common
    GTest::gtest
    GTest::gtest_main)
