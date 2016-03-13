cmake_minimum_required(VERSION 2.8)

option(DEBUG "Compile for debugging" OFF)

find_program(FLEX flex DOC "Path to the flex lexical analyser generator.")
if(NOT FLEX)
  message(FATAL_ERROR "Flex not found. Install the \"flex\" package")
endif()

find_program(BISON bison DOC "Path to the bison parser generator.")
if(NOT BISON)
  message(FATAL_ERROR "Bison not found. Install de \"bison\" package")
endif()

find_program(SPIM spim DOC "Used to simulate progam's execution")
if (NOT SPIM)
	message(SEND_ERROR "Spim no found. You will need a MIPS simulator in order to run the produced code")
endif()

project(not_hot)

set(CMAKE_BUILD_TYPE Release)
if (${DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
else (${DEBUG})
	set(CMAKE_BUILD_TYPE Release)
endif(${DEBUG})


set(CMAKE_CXX_FLAGS_DEBUG " -O0 -g -std=c++11 ")
set(CMAKE_CXX_FLAGS_RELEASE " -O3 -std=c++11 ")


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
