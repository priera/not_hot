include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tree")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tables")

add_library(tables STATIC ./implement/main_tables.cc ./implement/stringtab.cc)