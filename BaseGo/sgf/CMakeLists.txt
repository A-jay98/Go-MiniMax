include_directories("${PROJECT_SOURCE_DIR}/utils")


set(SGFGen_SOURCE_FILES sgfgen.cpp)
add_executable(SGFGen ${SGFGen_SOURCE_FILES})
target_link_libraries(SGFGen)


set(SGF_SOURCE_FILES sgf_utils.cpp sgfnode.cpp sgftree.h sgftree.cpp)
add_library(SGF ${SGF_SOURCE_FILES})
