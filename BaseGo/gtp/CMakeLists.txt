include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/core")
include_directories("${PROJECT_SOURCE_DIR}/engine")
include_directories("${PROJECT_SOURCE_DIR}/utils")

set(SOURCE_FILES gtp.cpp gtp.h play_gtp.cpp)
add_library(GTP ${SOURCE_FILES})
