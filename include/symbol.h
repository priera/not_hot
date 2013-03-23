#include "stanford-copyright.h"

#ifndef ENTRY_H
#define ENTRY_H

#include "cool-io.h"
#include <assert.h>
#include <string.h>

extern char *pad(int n);

class Entry;
typedef Entry* Symbol;

extern ostream& operator<<(ostream& s, const Entry& sym);
extern ostream& operator<<(ostream& s, Symbol sym);

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