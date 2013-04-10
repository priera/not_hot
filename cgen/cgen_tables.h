#ifndef CODIFIER_STR_H
#define CODIFIER_STR_H

#include <basic.h>
#include "stringtab.h"
#include "symtab.h"
#include "cgen_symbol.h"
#include "cool-tree.h"
#include "cgen_node.h"

namespace cgen {
	
	class CodifierStrTable {
	private:
		StrTable * tbl_;
		int class_tag_;
	public:
		CodifierStrTable(StrTable & tbl, int class_tag); 
		
		void code(ostream & str);
	};
	
	class CodifierIntTable {
	private:
		IntTable * tbl_;
		int class_tag_;
	public:
		CodifierIntTable(IntTable & tbl, int class_tag);
		
		void code(ostream & str);
	};
	
	class CgenNodesTable {
	private:
		List<CgenNode> * last_node_;
		List<CgenNode> * first_node_;
		CgenNodesTable();
		
	public:
		static CgenNodesTable & getInstance();
		
		BasicListIterator<CgenNode> * getIterator();
		
		CgenNode * findNode(Symbol name);
		void addNode(CgenNode & nd);
	};
	
	class CasesTable {
	private:
		int next_case_;
		List<typcase_class> * cases_;
		
		CasesTable();
	
	public:
		static CasesTable & getInstance();
		
		int get_next_case_index();
		
		BasicListIterator<typcase_class> * getIterator();
		
		void addCase(typcase_class & c);
		
	};
	
	class MethodsOverridingTable : public SymbolTable<Entry *, CgenMethod> {
	private:
		MethodsOverridingTable();
	public:
		static MethodsOverridingTable & getInstance();
	};

	class IdentifierAccess {
	public:
		enum IDENTIFER_ACCESS { LOCAL, PARAMETER, MEMBER, SELF} access_;
		int fpOffset_;
		
		IdentifierAccess(IDENTIFER_ACCESS access);
		IdentifierAccess(IDENTIFER_ACCESS access, int fpOffset);
	};
	
	class IdentifierAccessTable : public SymbolTable<Symbol, IdentifierAccess> {
	private:
		int FP_position_;
		
		IdentifierAccessTable();
	
	public: 
		static IdentifierAccessTable & getInstance();
		
		int increaseFPPosition();
		void decreaseFPPosition();
		void resetFPPosition();
	};
	
} 

#endif