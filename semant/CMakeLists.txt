include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tree")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tables")
include_directories("${PROJECT_SOURCE_DIR}/semant")

add_library(semant STATIC ./implement/inheritance_checker.cc ./implement/semant_driver.cc )