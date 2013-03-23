#ifndef INHERITANCE_CHECKER_H
#define INHERITANCE_CHECKER_H

#include "cool-tree.h"
#include "main_tables.h"

namespace semant {
	
	class CheckingInheritanceError {
	public:
		std::string message_;
		Class_ c_;
		
		CheckingInheritanceError(Class_ c, std::string message);
		CheckingInheritanceError(std::string message);
		CheckingInheritanceError(ClassRegisteringError err);
		
		std::string toString();
	};
	
	class InheritanceErrorMngr {
	private:
		InheritanceErrorMngr();
		
		List<CheckingInheritanceError> * errors_;
		
	public:
		static InheritanceErrorMngr & getInstance();
		
		void registerError(CheckingInheritanceError * err);
		List<CheckingInheritanceError> * get_errors_list();
		
	};
	
	class InheritanceChecker {
	private:
		
		ClassTable & classtable;
		Classes user_classes;
		
		bool trySetAsChild(Class_ c);
		void setAsChild(Class_ c, Classes &orphan_childs);
		void setOrphanChildsParents(Classes orphans);
		
		void buildInheritanceGraph();
	public:
		InheritanceChecker(Classes classes_);
		
		void check();
	};
	
} 

#endif