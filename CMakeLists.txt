cmake_minimum_required(VERSION 2.8)

find_program(FLEX flex DOC "Path to the flex lexical analyser generator.")
if(NOT FLEX)
  message(SEND_ERROR "Flex not found.")
endif()

find_program(BISON bison DOC "Path to the bison parser generator.")
if(NOT BISON)
  message(SEND_ERROR "Bison not found.")
endif()

find_program(SPIM spim DOC "Used to simulate progam's execution")
if (NOT SPIM)
	message(SEND_ERROR "Spim no found")
endif()

project(not_hot)

add_subdirectory(lib) 
add_subdirectory(parsing)
add_subdirectory(semant)
add_subdirectory(cgen)

include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tree")
include_directories("${PROJECT_SOURCE_DIR}/lib/main_structures/tables")
include_directories("${PROJECT_SOURCE_DIR}/parsing")
include_directories("${PROJECT_SOURCE_DIR}/semant")
include_directories("${PROJECT_SOURCE_DIR}/cgen")

add_executable(not_hot driver.cc)

target_link_libraries (not_hot basic)
target_link_libraries (not_hot tree)
target_link_libraries (not_hot tables)
target_link_libraries (not_hot parsing)
target_link_libraries (not_hot semant)
target_link_libraries (not_hot cgen)

install(TARGETS not_hot RUNTIME DESTINATION bin)

#install needed assembly files
file(MAKE_DIRECTORY ./build/as)
file(COPY ./resources/branch_resolver.s ./resources/trap.handler.s DESTINATION ./as)