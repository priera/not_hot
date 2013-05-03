#ifndef MAIN_TABLES_H
#define MAIN_TABLES_H

#include <basic.h>
#include "stringtab.h"
#include "symtab.h"

//Forward definition of tree classes used below
#include "cool_tree_forward.h"

class ObjectEnvironment : public SymbolTable<Symbol, Entry> {
private:
	ObjectEnvironment();
public:
	static ObjectEnvironment & getInstance();
}; 

class MethodSignature_ {
public:
	Formals formals;
	Symbol returnType;
	MethodSignature_ (Formals f_, Symbol rt);
};

typedef MethodSignature_ *MethodSignature;

class MethodSignatureKey_ {
public:
	Class_ c;
	Symbol f;
	MethodSignatureKey_ (Class_ c_, Symbol f_);
	
	ostream & print(ostream & s) const;
	
	bool operator==(const MethodSignatureKey_ &other);
};

typedef MethodSignatureKey_ *SignatureKey;

class ClassMethods : public SymbolTable <MethodSignatureKey_, MethodSignature_> {
private:
	ClassMethods();
public:
	static ClassMethods & getInstance();
	
	MethodSignature_ * getSignature(MethodSignatureKey_ mk);
};

class ClassRegisteringError {
public:
	Class_ c_;
	std::string message_;
	ClassRegisteringError(Class_ c, std::string message);
};

class ClassTable : public SymbolTable<Entry *, Class__class> {
private:
	
	Classes user_classes;
	Classes all_classes;
	Classes predefined_classes;
	
	Class_ base_class;
	Class_ main_class;
	
	bool main_found;
	
	Classes generateBasicClasses();
	Symbol climbingLub(Symbol t1, Symbol t2);
	
	ostream & print_class_hierarchy(ostream & stream, Class_ c, int tab);
	
	static Symbol const Object;
	
	ClassTable();
	
public:
	
	static ClassTable & getInstance();
	
	Classes get_user_classes();
	Classes get_all_classes();
	Classes get_predefined_classes();
	Class_ get_base_class();
	
	bool hasMainClass();
	bool validTypeName(Symbol s);
	
	ClassRegisteringError * add_class(Class_ c);
	bool isSubtypeOf(Symbol subtype, Symbol supertype);
	Symbol lub(Symbol t1, Symbol t2);
	
	ostream & print(ostream & stream);
	
};

//After declaring these classes, we can now include the tree classes declarations
#include "cool-tree.h"

#endif
