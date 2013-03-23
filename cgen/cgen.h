#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

enum ScopeTypeElem { ATTRIBUTE, PARAM, LOCAL }; 
typedef ScopeTypeElem *ScopeTypeElemP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

//class CgenNode;
//typedef CgenNode *CgenNodeP;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
	int objectclasstag;
	int ioclasstag;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;
	int next_class_tag;

// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);

	bool has_static_class(CgenNodeP nd);
	void set_static_class(CgenNodeP nd);

	void collect_proto_table_info(CgenNodeP nd);

	void emit_attr_def(attr_class *attr);
	void emit_prototable(CgenNodeP nd);
	void emit_prototables();
	void emit_class_nameTab();
	void emit_dispatch_tables();
	void emit_dispatch_table_content(CgenNodeP nd);

	int get_next_class_tag(){ next_class_tag++; return next_class_tag; }
	
	void code_initializers(CgenNodeP nd);
	void code_init_body(CgenNodeP nd);

	bool class_yet_coded(CgenNodeP nd);
	void code_methods(CgenNodeP nd);
public:

	StringEntryP empty_string;
	IntEntryP empty_int;

   CgenClassTable(Classes, ostream& str);
	
   void code();
   CgenNodeP root();
};

class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
				                                 // `NotBasic' otherwise
	List<attr_class> *all_attributes;
	List<attr_class> *own_attributes;
	List<method_class> *all_methods;
	List<method_class> *own_methods;
	int classtag;
	int size;
	int attr_count;
	int method_count;
	bool features_collected;
public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

	void collect_features();

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
	
	int get_classtag() {return classtag; }
	void set_classtag(int classtag_) { classtag = classtag_; } 
	int get_size() {return size; };	
	void set_object_size();
	List<attr_class> *get_all_attributes(){return all_attributes; };
	List<attr_class> *get_own_attributes(){ return own_attributes; };
	List<method_class> *get_all_methods(){return all_methods; };
	List<method_class> *get_own_methods() {return own_methods; };

	attr_class *get_attribute_by_name(Symbol name){
		bool found = false;
		attr_class *ret = NULL;
		List <attr_class> *l = this->all_attributes;
		while (!found && l){
			if (l->hd()->name == name) {
				found = true;
				ret = l->hd();
			} 
			l = l->tl();
		}
		return ret;
	}
};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};

class ScopeClass : public SymbolTable<Symbol, ScopeTypeElem> {
public:

	List<Entry> *locals;
	ScopeClass() : SymbolTable<Symbol, ScopeTypeElem>() {
		locals = NULL;
	}

	void push_class_attributes(CgenNodeP nd){
		this->enterscope();
		attr_class *temp;
		ScopeTypeElem type = ATTRIBUTE;
		ScopeTypeElemP type_obj = &type;
		for (List<attr_class> *l = nd->get_all_attributes(); l; l = l->tl()){
			temp = l->hd();
			this->addid(temp->name, type_obj);
		}
	}
	void pop_class_attributes(){
		this->exitscope();
	}

	void push_formals(method_class *m){
		this->enterscope();
		ScopeTypeElem type = PARAM;
		ScopeTypeElemP type_obj = &type;
		formal_class *temp;
		for (int i = m->formals->first(); m->formals->more(i); i = m->formals->next(i)){
			temp = (formal_class *)m->formals->nth(i);
			this->addid(temp->name, type_obj);
		}
	}

	void pop_formals() {
		this->exitscope();
	}

	void push_local(Symbol s){
		this->enterscope();
		ScopeTypeElem type = LOCAL;
		ScopeTypeElemP type_obj = &type;
		this->addid(s, type_obj);
		this->locals = new List<Entry>(s, this->locals);
	}

	void pop_local(){
		this->exitscope();

		//remove first local from list
		if (this->locals) this->locals = this->locals->tl();
	}

	void lookup_variable(Symbol s, ScopeTypeElemP scope, int &stack_pos){
		scope = this->lookup(s);

		stack_pos = 1;
		if (*scope == LOCAL){
			bool found = false;
			List<Entry> *temp = this->locals;
			while (!found && temp){
				if (temp->hd() == s) {
					found = true;
				} else {
					stack_pos++;
					temp = temp->tl();
				}
			}
		}
	}
	
};

class LabelProvider {
private:
	int next_label;
public:
	LabelProvider() : next_label(-1) {
	;
	}
	
	int get_next_label() {
		return ++next_label;
	}
};
