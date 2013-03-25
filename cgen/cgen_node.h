#ifndef CGEN_NODE_H
#define CGEN_NODE_H

#include "stringtab.h"
#include "cool-tree.h"
#include "constants.h"
#include "emitter.h"

namespace cgen {

	class CgenAttribute;
	class CgenMethod;
	
	class CgenNode : public class__class {
	private: 
		CgenNode * parentnd_;                        // Parent of class
		
		List<CgenNode> *cgen_children_;                  // Children of class
		
		List<CgenAttribute> *attributes_;
		List<CgenAttribute> *own_attributes_;
		List<CgenMethod> *methods_;
		List<CgenMethod> *own_methods_;
		List<CgenMethod> *overridden_methods_;
		
		int classtag_;
		int size_;
		int attr_count_;
		int method_count_;
		
		int locals_in_constructor_;
		
		void set_classtag();
		void gather_children();
		void collect_features();
		void set_object_size();
		void set_locals_in_constructor();
		
		void emitDispatchTableContent(ostream & ouput);
		void codeInitBody(ostream & output);
		
		void registerAttributes();
		void codeInitializer(ostream & output);
		void codeMethods(ostream & output);
		
	public:
		CgenNode(class__class node, CgenNode * parent);
		
		void collect_info();
		void emitPrototables(ostream & output);
		void emitDispatchTable(ostream & ouput);
		
		void code(ostream & output);
		
		void inc_locals_count();
		
		CgenMethod * findMethod(Symbol method_name);
		List<CgenNode> * get_children_nodes();
		
	};
	
	class CgenAttribute : public attr_class {
	private:
		int offset_;
		CgenNode * class__;
		int locals_in_init_;
		
		void codeInitExpr(ostream & output);
		bool type_has_default_value();
		
	public:
		CgenAttribute(attr_class attr, CgenNode & class_, int offset);
		
		int get_offset();
		int get_locals_in_init();
		
		void codeInitialization(ostream & output);
		void emitDef(ostream & output);
		void emitStoreVal(std::string self_reg, ostream & s);
	};
	
	class CgenMethod : public method_class {
	private:
		int offset_;
		CgenNode * defining_class_;
		int locals_count_;
		
		void registerArguments();
		void countLocals();
	public:
		CgenMethod(method_class m, CgenNode & defining_class, int offset);
		
		int get_offset();
		void set_offset(int offset);
		void inc_locals_count();
		bool is_basic();
		
		CgenNode & get_defining_class();
		void code(ostream & output);
	};

	class CurrentCoding {
	private:
		CgenNode * current_class_;
		
		CurrentCoding();
	
	public:
		static CurrentCoding & getInstance();
		
		void set_current_class(CgenNode & class_);
		CgenNode * get_current_class();
		
	};
	
} 


//Previous classes had to be declared before including theses files
#include "expression_coder.h"
#include "cgen_tables.h"

#endif