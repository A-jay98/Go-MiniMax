include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/engine)

set(SOURCE_FILES core.cpp core.h info.h MiniMax.cpp MiniMax.h Node/Node.cpp Node/Node.h)
add_library(Core ${SOURCE_FILES})
