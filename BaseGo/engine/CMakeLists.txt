include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/gtp)
include_directories(${PROJECT_SOURCE_DIR}/sgf)
include_directories(${PROJECT_SOURCE_DIR}/utils)

set(SOURCE_FILES board.cpp board.h boardlib.cpp clock.cpp clock.h hash.cpp hash.h printutils.cpp)
add_library(Engine ${SOURCE_FILES})
