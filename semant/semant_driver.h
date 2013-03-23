#ifndef SEMANT_DRIVER_H
#define SEMANT_DRIVER_H

#include <iostream>  
#include "cool-io.h"
#include "cool-tree.h"
#include "stringtab.h"
#include "main_tables.h"
#include "inheritance_checker.h"

namespace semant {
	
	class SemantDriver {
	private:
		int semant_errors;
		ostream & error_stream;
		
		Classes user_classes;
		
		void checkUserClasses();
		void checkInheritance();
		void checkTypes();

		bool check_print_error();
		
		ostream& semant_error();
		ostream& semant_error(Class_ c, std::string message);
		ostream& semant_error(Symbol filename, tree_node *t, std::string message);
		
	public:
		
		ClassTable & classtable;
		
		SemantDriver(Classes classes_);
		
		void check_semantics();
		
		ostream& semant_error(ConstraintClassError er);
		
		int errors();
		
		Classes getAllClasses();
	};
	
}

#endif