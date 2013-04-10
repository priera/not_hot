include_directories("${PROJECT_SOURCE_DIR}/lib/basic")

add_library(basic STATIC ./implement/flags.cc ./implement/lineno_mng.cc ./implement/symbol.cc ./implement/utilities.cc ./implement/typing_errors.cc )