project(nartherion-gcc-arrival-time-filter)
add_library(${PROJECT_NAME} STATIC kalman_filter.cpp packet_group_tracker.cpp)
