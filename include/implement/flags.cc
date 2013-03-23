#include "flags.h"

using namespace flags;

/*Flag implementations*/
Flag::Flag() : s(INACTIVE) { }

void Flag::setState(FlagState s){
	this->s = s;
}

FlagState Flag::getState(){
	return s;
}

/*CompilerFlags implementations*/
CompilerFlags::CompilerFlags(int argc, char * argv[]){
	yy_flex_debug = new Flag();      
	cool_yydebug = new Flag();       
	lex_verbose = new Flag();        
	semant_debug = new Flag();       
	cgen_debug = new Flag();         
	disable_reg_alloc = new Flag();  
	
// 	cgen_Memmgr = GC_NOGC;
// 	cgen_Memmgr_Test = GC_NORMAL;  
// 	cgen_Memmgr_Debug = GC_QUICK;
      
	parseInput(argc,argv);
}

void CompilerFlags::parseInput(int argc, char *argv[]){
  int c;
  errorParsing = false;
  
	while ((c = getopt(argc, argv, "lpscvrOo:gtT")) != -1) {
		switch (c) {
#ifdef DEBUG
			case 'l':
				yy_flex_debug->setState(ACTIVE);
				break;
			case 'p':
				cool_yydebug->setState(ACTIVE);
				break;
			case 's':
				semant_debug->setState(ACTIVE);
				break;
			case 'c':
				cgen_debug->setState(ACTIVE);
				break;
			case 'v':
				lex_verbose->setState(ACTIVE);
				break;
			case 'r':
				disable_reg_alloc->setState(ACTIVE);
				break;
#else
			case 'l':
			case 'p':
			case 's':
			case 'c': 
			case 'v':
			case 'r':
				cerr << "No debugging available\n";
				break;
#endif
// 			case 'g':  // enable garbage collection
// 				cgen_Memmgr = GC_GENGC;
// 				break;
// 			case 't':  // run garbage collection very frequently (on every allocation)
// 				cgen_Memmgr_Test = GC_TEST;
// 				break;
// 			case 'T':  // do even more pedantic tests in garbage collection
// 				cgen_Memmgr_Debug = GC_DEBUG;
// 				break;
			case 'o':  // set the name of the output file
				out_filename = optarg;
				break;
			case 'O':  // enable optimization
				cgen_optimize->setState(ACTIVE);
				break;
			case '?':
				errorParsing = true;
				break;
			case ':':
				errorParsing= true;
				break;
		}
	}
}

bool CompilerFlags::wasError() {
	return errorParsing;
}
	

