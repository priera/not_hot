include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tree")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tables")
include_directories("${PROJECT_SOURCE_DIR}/parsing")

add_library(parsing STATIC ./driver/ParserDriver.cc ./lexer/lex.yy.cc ./parser/cool.tab.cc)

# Custom commands to invoke flex and bison.
add_custom_command(OUTPUT lex.yy.cc
                   COMMAND ${FLEX} cool.flex 
                   COMMENT "Generating lexer"
                   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/parsing/lexer
                   VERBATIM)

add_custom_command(OUTPUT cool.tab.cc
                   COMMAND ${BISON} cool.yy              
                   COMMENT "Generating parser"
                   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/parsing/parser
                   VERBATIM)
                   
add_custom_command(OUTPUT move_parser_headers 
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/parser/custom.cmake
	COMMENT "Moving bison generated headers"
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/parsing/parser
	MAIN_DEPENDENCY cool.tab.cc)
	
add_custom_target(move_parser_headers_cmd DEPENDS move_parser_headers)

add_dependencies(parsing move_parser_headers_cmd)
add_dependencies(parsing lex.yy.cc)

target_link_libraries(parsing basic)
target_link_libraries(parsing tables)
target_link_libraries(parsing tree)
