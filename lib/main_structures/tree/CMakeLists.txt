include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tree")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tables")

add_library(tree STATIC ./implement/cool-tree.cc ./implement/dumptype.cc ./implement/dump.cc ./implement/tree.cc)

target_link_libraries(tree basic)