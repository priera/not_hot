#include "inheritance_checker.h"

using namespace semant;

CheckingInheritanceError::CheckingInheritanceError(Class_ c, std::string message) : c_(c), message_(message) {}
CheckingInheritanceError::CheckingInheritanceError(std::string message) : c_(NULL), message_(message) {}
CheckingInheritanceError::CheckingInheritanceError(ClassRegisteringError err) : c_(err.c_), message_(err.message_) {}

std::string CheckingInheritanceError::toString() {
	std::string ret("");
	
	if (c_ != NULL) {
		std::ostringstream convert;
		convert << c_->get_line_number();
		ret += convert.str() + ": ";
	}
	
	ret += message_ + "\n";
}

InheritanceErrorMngr::InheritanceErrorMngr() : errors_(NULL) {};

InheritanceErrorMngr & InheritanceErrorMngr::getInstance() {
	static InheritanceErrorMngr mngr;
	return mngr;
}

void InheritanceErrorMngr::registerError(CheckingInheritanceError * err){
	
	if (errors_ == NULL) {
		errors_ = new List<CheckingInheritanceError>(err);
	} else {
		errors_ = new List<CheckingInheritanceError>(err, errors_);
	}
}

List<CheckingInheritanceError> * InheritanceErrorMngr::get_errors_list(){
	return errors_;
}

InheritanceChecker::InheritanceChecker(Classes classes_) : user_classes(classes_), classtable(ClassTable::getInstance()) { }

bool InheritanceChecker::trySetAsChild(Class_ c){
	bool ok = true;
	Symbol parent_symbol = c->get_parent();
	if (parent_symbol == GlobalTables::getInstance().get_constants().No_class) return ok;
	
	Class_ parent_class = classtable.probe(parent_symbol);
	
	if ( parent_class != NULL) {
		if (Class__class::isValidAncestor(parent_class)) {
			parent_class->append_child(c); 
		} else {
			InheritanceErrorMngr::getInstance().registerError(new CheckingInheritanceError(c, std::string("Cannot inherit from this base class")));
		}
	} else {
		ok = false;
	}
	
	return ok;
}

void InheritanceChecker::setAsChild(Class_ c, Classes &orphan_childs){
	if (!trySetAsChild(c)){
		orphan_childs = append_Classes(single_Classes(c), orphan_childs);
	}
}

void InheritanceChecker::setOrphanChildsParents(Classes orphans){
	for (int i = orphans->first(); orphans->more(i); i = orphans->next(i)){
		trySetAsChild(orphans->nth(i));
	}
}

void InheritanceChecker::buildInheritanceGraph(){
	Classes orphan_childs = nil_Classes();
	Classes all_classes = append_Classes(classtable.get_predefined_classes(), user_classes);
	Class_ current_class;
	
	bool classError = false;
	int i = all_classes->first();
	ClassRegisteringError * class_error;
	
	while (!classError && all_classes->more(i)) {
		
		current_class = all_classes->nth(i);
		class_error = classtable.add_class(current_class);
		
		if (class_error != NULL) {
			InheritanceErrorMngr::getInstance().registerError(new CheckingInheritanceError(*class_error));
			classError = true;
		}
		
		setAsChild(current_class, orphan_childs);
		
		i = all_classes->next(i);
	}
	
	setOrphanChildsParents(orphan_childs);
	
}

void InheritanceChecker::check(){
	
	//STEP 1: Build inheritance structure
	buildInheritanceGraph();
	
	//STEP 2: Mark all classes which inherits from OBJECT
	ok ok = classtable.get_base_class()->markSubclasses();
	//Really, following error won't raise in practice, because the grammar, and previous checks, prevent it
	if (!ok) InheritanceErrorMngr::getInstance().registerError(new CheckingInheritanceError(std::string("Cycled inheritance hierarchy found")));
	
	//STEP 3: Look for any class which hasn't been marked. Those are invalid. This algorithm also handles when a class inherits from itself
	bool found = false;
	Classes classes = classtable.get_all_classes(); 
	int i = classes->first();
	while (!found && classes->more(i)){
		if (classes->nth(i)->state == Class__class::NOT_MARKED){
			InheritanceErrorMngr::getInstance().registerError(new CheckingInheritanceError(classes->nth(i), \
			std::string("Cycled inheritance hierarchy found")));
			found = true;
		}	
		i = classes->next(i);
	}
	
}