#ifndef CGEN_DRIVER_H
#define CGEN_DRIVER_H 

#include <basic.h>
#include "emitter.h"
#include "stringtab.h"
#include "main_tables.h"

#include "cgen_node.h"
#include "cgen_tables.h"

namespace cgen {
	
	class Driver {
	private:
		ostream & output_;
		CgenNode * baseNodeClass_;

		void codeGlobalData();
		void codeSelectGc();
		void codeConstantTables();
		void codeBools();
		void emitClassNameTab();
		void emitClassTab();
		void emitCasesTab();
		void emitProtObjTab();
		void codeGlobalText();
		void codeInitializers();
		
		char * getCharPointer(string str);
		
	public:
		Driver(ostream & output);
		
		void code();
	};
	
} 

#endif