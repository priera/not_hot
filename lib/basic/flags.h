#ifndef FLAGS_H
#define FLAGS_H

#include <stdio.h>
#include <unistd.h>

#include "cool-io.h"
//#include "cgen_gc.h"

namespace flags{
	
	class GarbageCollectionOptions {
		public:
			enum Memmgr { GC_NOGC, GC_GENGC, GC_SNCGC } cgen_Memmgr;
		
			enum Memmgr_Test { GC_NORMAL, GC_TEST } cgen_Memmgr_Test;
		
			enum Memmgr_Debug { GC_QUICK, GC_DEBUG } cgen_Memmgr_Debug;
	};
	enum FlagState{
		INACTIVE,
		ACTIVE
	};
	
	class Flag {
	private:
		FlagState s;
	public:
		Flag();
		void setState(FlagState state);
		FlagState getState();
	};
	
	class CompilerFlags {
	private: 
		bool errorParsing;
		
		void parseInput(int argc, char *argv[]);
	public:
		Flag *yy_flex_debug;       // for the lexer; prints recognized rules
		Flag *cool_yydebug;        // for the parser
      Flag *lex_verbose;         // also for the lexer; prints tokens
      Flag *semant_debug;        // for semantic analysis
      Flag *cgen_debug;          // for code gen
      Flag *disable_reg_alloc;  // Don't do register allocation

      Flag *cgen_optimize;       	// optimize switch for code generator 
      char *out_filename;     	 // file name for generated code
//       Memmgr cgen_Memmgr;  		// enable/disable garbage collection
//       Memmgr_Test cgen_Memmgr_Test;  // normal/test GC
//       Memmgr_Debug cgen_Memmgr_Debug;// check heap frequently
//       
      CompilerFlags(int argc, char *argv[]);
		
		bool wasError();
	};
	
} 

#endif