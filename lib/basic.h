#ifndef BASIC_H
#define BASIC_H

#include <ctype.h>       // for isprint
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

# include <iostream>

using std::ostream;
using std::cout;
using std::cerr;
using std::endl;

# include <fstream>

using std::ofstream;

# include <iomanip>

using std::oct;
using std::dec;
using std::setw;
using std::setfill;

namespace flags{

	class GarbageCollectionOptions {
		public:
			enum Memmgr { GC_NOGC, GC_GENGC, GC_SNCGC } cgen_Memmgr;
		
			enum Memmgr_Test { GC_NORMAL, GC_TEST } cgen_Memmgr_Test;
		
			enum Memmgr_Debug { GC_QUICK, GC_DEBUG } cgen_Memmgr_Debug;
	};
	
	enum FlagState{
		INACTIVE,
		ACTIVE
	};
	
	class Flag {
	private:
		FlagState s;
	public:
		Flag();
		void setState(FlagState state);
		FlagState getState();
	};
	
	class CompilerFlags {
	private: 
		bool errorParsing;
		
		void parseInput(int argc, char *argv[]);
	public:
		Flag *yy_flex_debug;       // for the lexer; prints recognized rules
		Flag *cool_yydebug;        // for the parser
      Flag *lex_verbose;         // also for the lexer; prints tokens
      Flag *semant_debug;        // for semantic analysis
      Flag *cgen_debug;          // for code gen
      Flag *disable_reg_alloc;  // Don't do register allocation

      Flag *cgen_optimize;       	// optimize switch for code generator 
      char *out_filename;     	 // file name for generated code
//       Memmgr cgen_Memmgr;  		// enable/disable garbage collection
//       Memmgr_Test cgen_Memmgr_Test;  // normal/test GC
//       Memmgr_Debug cgen_Memmgr_Debug;// check heap frequently
//       
      CompilerFlags(int argc, char *argv[]);
		
		bool wasError();
	};
	
} 

class Lineno_Mngr{
public:
	static int curr_lineno;
	
	static void reset();
};

template <class T>
class BasicListIterator;

template <class T>
class List {
private:
	T *head;
	List<T>* tail;
public:
	List(T *h,List<T>* t = NULL): head(h), tail(t) { }

	T *hd() const       { return head; }  
	List<T>* tl() const { return tail; }
  
	BasicListIterator<T> * getIterator() {
		return new BasicListIterator<T>(this);
	}
  
	List<T>* reverse()  {
		List<T> *ret = NULL;
		for (List<T> * l = this; l; l = l->tl()){
			ret = new List<T>(l->hd(), ret);
		}
		
		return ret;
	}
	
	void add(T & end){
		tail = new List<T>(&end);
	}
	
	static void remove(List<T> * &l, T * elem){
		List<T> * previous = NULL;
		List<T> * iter = l;
		bool found = false;
		while (!found && iter){
			if (iter->hd() == elem) {
				found = true;
			} else {
				previous = iter;
				iter = iter->tl();
			}
		}
		
		if (found) {
			if (previous == NULL){
				l = l->tail;
			} else {
				previous->tail = iter->tail;
			}
		}
	}
	
	static void replace(List<T> * &l, T * old, T * new_){
		List<T> * previous = NULL;
		List<T> * iter = l;
		bool found = false;
		while (!found && iter){
			if (iter->hd() == old) {
				found = true;
			} else {
				previous = iter;
				iter = iter->tl();
			}
		}
		
		if (found) {
			if (previous == NULL){
				l = new List<T>(new_, l->tail);
			} else {
				previous->tail = new List<T>(new_,iter->tail);
			}
		}
	}
	
	//Performs a deep-copy of the list passed as argument, returning a 
	//brand-new list, containing the same number of elements, in the same order, and using the same pointers. 
	//Because this list class is not really object-oriented, a copy constructor hasn't been used for this purpose.
	List<T> * copy(List<T> * &last_node) {
		List<T> *ret = NULL;
		last_node =NULL;
		for (List<T> * l = this; l; l = l->tl()){
			if (ret == NULL) {
				ret = new List<T>(l->hd(), NULL);
				last_node = ret;
			} else {
				last_node->tail = new List<T>(l->hd(), NULL);
				last_node= last_node->tail;
			}
		}
		
		return ret;
	}
	
	List<T> * copy () {
		List<T> * node;
		return copy(node);
	}
	
	static List<T> * concat (List<T> * begin, List<T> * end){
		
		if (begin == NULL) {
			return end;
		}
		
		List<T> * last_node = NULL;
		for (List<T> * l = begin; l; l = l->tl()){
			if (l->tl() != NULL){
				last_node = l;
			}
		}
		
		last_node->tail = end;
		
		return begin;
	}
};

template <class T>
class BasicListIterator {
private:
	List<T> * list_;
	
public:
	BasicListIterator(List<T> * l) : list_(l) {}
	
	bool more(){
		return list_ != NULL;
	}
	void move(){
		list_ = list_->tl();
	}
	
	T * current(){
		return list_->hd();
	}
};

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



/////////////////////////////////////////////////////////////////////////
// 
// list function templates
//
// To avoid potential problems with mutliple definitions of 
// the List<> class members, the list functions are not members of the
// list class.
//
/////////////////////////////////////////////////////////////////////////

//
// Map a function for its side effect over a list.
//
template <class T>
void list_map(void f(T*), List<T> *l)
{
  for (l; l != NULL; l = l->tl())
    f(l->hd());
}

//
// Print the given list on the standard output.
// Requires that "<<" be defined for the element type.
//
template <class S, class T>
void list_print(S &str, List<T> *l)
{
   str << "[\n";
   for(; l != NULL; l = l->tl())
	str << *(l->hd()) << " ";
   str << "]\n";
}

//
// Compute the length of a list.
//
template <class T>
int list_length(List<T> *l)
{
  int i = 0;
  for (; l != NULL; l = l->tl())
    i++;
  return i;
}

#ifndef BASIC_ENTRY_H
#define BASIC_ENTRY_H

class Entry;
typedef Entry* Symbol;

ostream& operator<<(ostream& s, const Entry& sym);
ostream& operator<<(ostream& s, Symbol sym);

/////////////////////////////////////////////////////////////////////////
//
//  String Table Entries
//
/////////////////////////////////////////////////////////////////////////

class Entry {
protected:
	char *str;     // the string
	int  len;      // the length of the string (without trailing \0)
	int index;     // a unique index for each string
public:
	Entry(char *s, int l, int i);
	
	// is string argument equal to the str of this Entry?
	int equal_string(char *s, int len) const;  
	
	// is the integer argument equal to the index of this Entry?
	bool equal_index(int ind) const           { return ind == index; }
	
	ostream& print(ostream& s) const;
	
	// Return the str and len components of the Entry.
	char *get_string() const;
	int get_len() const;
};

//
// There are three kinds of string table entries:
//   a true string, an string representation of an identifier, and 
//   a string representation of an integer.
//
// Having separate tables is convenient for code generation.  Different
// data definitions are generated for string constants (StringEntry) and 
// integer  constants (IntEntry).  Identifiers (IdEntry) don't produce
// static data definitions.
//
// code_def and code_ref are used by the code to produce definitions and
// references (respectively) to constants.  
//
class StringEntry : public Entry {
public:
	StringEntry(char *s, int l, int i);
};

class IdEntry : public Entry {
public:
	IdEntry(char *s, int l, int i);
};

class IntEntry: public Entry {
public:
	IntEntry(char *s, int l, int i);
};

typedef StringEntry *StringEntryP;
typedef IdEntry *IdEntryP;
typedef IntEntry *IntEntryP;

#endif

void fatal_error(char *);
void print_escaped_string(ostream& str, const char *s);
char * pad(int);
char * changeCase(char * a); 

bool copy_Boolean(bool b);
void assert_Boolean(bool);
void dump_Boolean(ostream& stream, int padding, bool b);

Symbol copy_Symbol(Symbol);
void assert_Symbol(Symbol);
void dump_Symbol(ostream &,int,Symbol);

#endif