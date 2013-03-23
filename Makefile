export CC=g++

export DIR_PROJECT=$(shell pwd -P)/
export DIR_BUILD=${DIR_PROJECT}build/
DIR_MAKEFILES=${DIR_PROJECT}makefiles/

SUBDIRS =include_ parsing semant cgen

DIR_INCLUDES_H=${DIR_PROJECT}include/
DIR_TREE_H=${DIR_PROJECT}include/tree/
DIR_PARSER_H=${DIR_PROJECT}parsing/
DIR_TABLES_H=${DIR_PROJECT}include/tables/
DIR_SEMANT_H=${DIR_PROJECT}semant/
DIR_CGEN_H=${DIR_PROJECT}cgen/

CFLAGS= -I${DIR_INCLUDES_H} -I${DIR_PARSER_H} -I${DIR_TREE_H} -I${DIR_TABLES_H} -I${DIR_SEMANT_H} -I${DIR_CGEN_H}

CFIL= driver.cc 
OBJS=${CFIL:.cc=.o}

ALL_OBJS=$(shell echo ${DIR_BUILD}*.o) 

# MAIN_BUILD_DEPS= build_environment include_src parsingUtilities driver
MAIN_BUILD_DEPS= build_environment ${SUBDIRS} driver

.PHONY: coolc test build_environment ${SUBDIRS} driver

coolc: ${MAIN_BUILD_DEPS}
	${CC} ${CFLAGS} ${ALL_OBJS} -o ${DIR_BUILD}coolc

test: coolc 
	./build/coolc ./build/tests/good.cl
	
build_environment: 
	mkdir -p ${DIR_BUILD}
	
${SUBDIRS}:
	@echo "Building" $@
	cd ${DIR_MAKEFILES} ; $(MAKE) -f $@
	
include_src:
	@echo "Building includes"
	cd ${DIR_MAKEFILES} ; $(MAKE) -f include_src
	
parsingUtilities:
	@echo "Building parsing utilities"
	cd ${DIR_MAKEFILES} ; $(MAKE) -f parsingUtilities
	
driver: ${OBJS}
	
%.o: %.cc 
	${CC} ${CFLAGS} -c $< -o $(addprefix ${DIR_BUILD}, $@)