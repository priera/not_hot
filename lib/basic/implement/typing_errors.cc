#include "typing_errors.h"

void TypingErrorEnvironment::set_filename(const std::string & filename){
	filename_ = filename;
}
void TypingErrorEnvironment::set_classname(const std::string & classname) {
	classname_ = classname;
}
void TypingErrorEnvironment::set_scope(TypingErrorEnvironment::TYPING_SCOPE scope) { scope_ = scope; }

std::string TypingErrorEnvironment::get_filename() {return filename_; }
std::string TypingErrorEnvironment::get_classname() { return classname_; }
TypingErrorEnvironment::TYPING_SCOPE TypingErrorEnvironment::get_scope() { return scope_; }

TypingErrorEnvironment & TypingErrorEnvironment::getCurrentEnvironment() {
	static TypingErrorEnvironment E;
	return E;
}

TypingErrorEnvironment::TypingErrorEnvironment() {};

TypingErrorInfo::TypingErrorInfo(TypingErrorEnvironment environment, std::string message, int lineno)  { 
	filename_ = environment.get_filename();
	classname_ = environment.get_classname();
	scope_ = environment.get_scope();
	lineno_ = lineno;
	message_ = message;
}

std::string TypingErrorInfo::toString() {
	std::ostringstream convert;
	convert << lineno_;
	
	//std::string msg = "In file: ";
	std::string msg = filename_ + ":" + convert.str() + ":";
	//msg += filename_ + " at line: " + convert.str();
	msg += "\t(class: " + classname_ + "). "; 
 	msg += "Error: " + message_;
	return msg;
}

TypingErrorMngr::TypingErrorMngr() : typing_errors_(NULL) { }

TypingErrorMngr & TypingErrorMngr::getInstance() {
	static TypingErrorMngr TEM;
	return TEM;
}

void TypingErrorMngr::registerError(std::string message, int line_number){
	TypingErrorInfo * info = new TypingErrorInfo(TypingErrorEnvironment::getCurrentEnvironment(), message, line_number);
	
	if (typing_errors_ == NULL) {
		typing_errors_ = new List<TypingErrorInfo>(info);
	} else {
		typing_errors_ = new List<TypingErrorInfo>(info, typing_errors_);
	}
}

List<TypingErrorInfo> * TypingErrorMngr::get_errors_list(){
	return typing_errors_;
};