include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tree")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tables")
include_directories("${PROJECT_SOURCE_DIR}/cgen")

add_library(cgen STATIC ./implement/cgen_driver.cc ./implement/cgen_node.cc ./implement/cgen_symbol.cc ./implement/cgen_tables.cc ./implement/constants.cc 
 ./implement/emitter.cc ./implement/expression_coder.cc)