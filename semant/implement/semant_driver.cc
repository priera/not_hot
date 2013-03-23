#include "semant_driver.h"

namespace semant {
	
	SemantDriver::SemantDriver(Classes classes_) : error_stream(cerr), user_classes(classes_), classtable(ClassTable::getInstance()) { }
	
	bool SemantDriver::check_print_error(){
		bool ret = (errors() > 0);
		if (ret) {
			cerr << "Compilation halted due to static semantic errors." << endl;
		}
		return ret;
	}

	void SemantDriver::checkUserClasses() {
		
		ConstraintClassError * error = NULL;
		
		classtable.get_base_class()->checkSemantics(NULL, error);
		
		if (error != NULL) {
			semant_error(*error);
		}
		
		if (!classtable.hasMainClass()){
			semant_error() << "Class Main is not defined." << endl;
		}
		
	}

	void SemantDriver::checkInheritance(){
		InheritanceChecker inh_checker(user_classes);
		
		inh_checker.check();
		
		List<CheckingInheritanceError> * errors = InheritanceErrorMngr::getInstance().get_errors_list();
		
		if (errors != NULL) {
			semant_error();
			
			for (List<CheckingInheritanceError> * l = errors; l != NULL ; l = l->tl()){
				error_stream << l->hd()->toString() << endl;
			} 
		}
	}
	
	void SemantDriver::checkTypes(){
		Classes classes = classtable.get_all_classes();
		
		classtable.get_base_class()->checkTypes(ObjectEnvironment::getInstance());
		
		List<TypingErrorInfo> * typing_errors = TypingErrorMngr::getInstance().get_errors_list();

		if (typing_errors != NULL) {
			semant_error();
			
			for (List<TypingErrorInfo> * l = typing_errors; l != NULL ; l = l->tl()){
				error_stream << l->hd()->toString() << endl;
			} 
		}
	}
	
	void SemantDriver::check_semantics(){
		
		checkInheritance();
		if(check_print_error()){
			exit(1);
		}
		
		checkUserClasses();
		if(check_print_error()){
			exit(1);
		}
		
		checkTypes();
		if(check_print_error()){
			exit(1);
		}
	}
	
	int SemantDriver::errors() { return semant_errors; }
	
	Classes SemantDriver::getAllClasses() {
		return classtable.get_all_classes();
	}
	
	////////////////////////////////////////////////////////////////////
	//
	// semant_error is an overloaded function for reporting errors
	// during semantic analysis.  There are three versions:
	//
	//    ostream& SemantDriver::semant_error()                
	//
	//    ostream& SemantDriver::semant_error(Class_ c)
	//       print line number and filename for `c'
	//
	//    ostream& SemantDriver::semant_error(Symbol filename, tree_node *t)  
	//       print a line number and filename
	//
	///////////////////////////////////////////////////////////////////

// 	ostream& SemantDriver::semant_error(ClassRegisteringError er){
// 		return semant_error(er.c_, er.message_);
// 	}
	
	ostream& SemantDriver::semant_error(ConstraintClassError er){
		return semant_error(er.c_, er.message_);
	}
	
	ostream& SemantDriver::semant_error(Class_ c, std::string message)
	{                                                             
		return semant_error(c->get_filename(),c, message);
	}    
	
	ostream& SemantDriver::semant_error(Symbol filename, tree_node *t, std::string message)
	{
		error_stream << filename << ":" << t->get_line_number() << ": " << message << endl;
		return semant_error();
	}
	
	ostream& SemantDriver::semant_error()                  
	{                                                 
		semant_errors++;                            
		return error_stream;
	} 
	
}
