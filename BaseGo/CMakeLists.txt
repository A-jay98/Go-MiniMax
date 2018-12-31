cmake_minimum_required(VERSION 3.6)
project(BaseGo)

set(CMAKE_C_STANDARD 99)

include_directories("${PROJECT_SOURCE_DIR}/core")
include_directories("${PROJECT_SOURCE_DIR}/engine")
include_directories("${PROJECT_SOURCE_DIR}/gtp")
include_directories("${PROJECT_SOURCE_DIR}/sgf")
include_directories("${PROJECT_SOURCE_DIR}/utils")

add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(gtp)
add_subdirectory(sgf)
add_subdirectory(utils)

set(SOURCE_FILES main.cpp)
add_executable(BaseGo ${SOURCE_FILES})

set(LIBRARIES Core Engine GTP SGF Utils)
target_link_libraries(BaseGo ${LIBRARIES})
