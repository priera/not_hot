#ifndef CGEN_SYMBOL_H
#define CGEN_SYMBOL_H

#include "symbol.h"
#include "stringtab.h"

namespace cgen {
	
	namespace symbol {
		
		class CgenStringEntry : public StringEntry {
		public:
			CgenStringEntry(StringEntry & entry);
			
			static CgenStringEntry & empty();
			
			void code_def(ostream& s, int stringclasstag);
			void code_ref(ostream& s) const;
			
		};
		
		class CgenIntEntry : public IntEntry {
		public:
			CgenIntEntry(IntEntry & entry);
			
			static CgenIntEntry & empty();
			
			void code_def(ostream& s, int intclasstag);
			void code_ref(ostream& s) const;
		};
		
		class BoolConst {
		private: 
			int val;
			BoolConst(int);
		public:
			static BoolConst & true_();
			static BoolConst & false_();
			
			static BoolConst & getProperBool(bool val);
			
			void code_def(ostream&, int boolclasstag);
			void code_ref(ostream&) const;
		};
		
	}

} 

#include "emitter.h"

#endif