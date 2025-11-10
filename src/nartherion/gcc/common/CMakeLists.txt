project(nartherion-gcc-common)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${NARTHERION_GCC_SOURCE_DIR}/include)

include(${NARTHERION_GCC_SOURCE_DIR}/cmake/compiler_warnings.cmake)
nartherion_gcc_set_compiler_warnings(${PROJECT_NAME})
