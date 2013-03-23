#ifndef TYPING_ERROR_H
#define TYPING_ERROR_H

#include <string>
#include <sstream>
#include "list.h"
#include "cool-io.h"

 class TypingErrorEnvironment{
 public:
	 enum TYPING_SCOPE {method, attribute};
	 
	 void set_filename(const std::string & filename);
	 void set_classname(const std::string & classname);
	 void set_scope(TYPING_SCOPE scope);
	 
	 std::string get_filename();
	 std::string get_classname();
	 TYPING_SCOPE get_scope();
	 
	 static TypingErrorEnvironment & getCurrentEnvironment();
 private:
	 std::string filename_;
	 std::string classname_;
	 TYPING_SCOPE scope_;
	 
	 TypingErrorEnvironment();
 };
 
 class TypingErrorInfo {
 private:
	 std::string filename_;
	 std::string classname_;
	 int lineno_;
	 std::string message_;
	 TypingErrorEnvironment::TYPING_SCOPE scope_;
 public:
	 TypingErrorInfo(TypingErrorEnvironment environment, std::string message, int lineno);
	 
	 std::string toString();
 };

class TypingErrorMngr {
private:
	TypingErrorMngr();
	
	List<TypingErrorInfo> * typing_errors_;
	
public:
	static TypingErrorMngr & getInstance();
	
	void registerError(std::string message, int line_number);
	List<TypingErrorInfo> * get_errors_list();
	
};

#endif