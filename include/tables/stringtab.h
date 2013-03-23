// -*-Mode: C++;-*-
//
// See stanford-copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "stanford-copyright.h"

#ifndef _STRINGTAB_H_
#define _STRINGTAB_H_

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "cool-io.h"
#include "list.h"    // list template
#include "symbol.h"

//////////////////////////////////////////////////////////////////////////
//
//  String Tables
//
//////////////////////////////////////////////////////////////////////////

template <class Elem> 
class StringTable
{
protected:
   List<Elem> *tbl;   // a string table is a list
   int index;         // the current index
public:
   StringTable(): tbl((List<Elem> *) NULL), index(0) { }   // an empty table
   // The following methods each add a string to the string table.  
   // Only one copy of each string is maintained.  
   // Returns a pointer to the string table entry with the string.

   // add the prefix of s of length maxchars
   Elem *add_string(char *s, int maxchars);

   // add the (null terminated) string s
   Elem *add_string(char *s);

   // add the string representation of an integer
   Elem *add_int(int i);


   // An iterator.
   int first();       // first index
   int more(int i);   // are there more indices?
   int next(int i);   // next index

   Elem *lookup(int index);      // lookup an element using its index
   Elem *lookup_string(char *s); // lookup an element using its string

   void print();  // print the entire table; for debugging

};

class IdTable : public StringTable<IdEntry> { };

class StrTable : public StringTable<StringEntry>
{
public: 
	StringEntry *add_string(char *s);
};

class IntTable : public StringTable<IntEntry>
{
};

// IdTable idtable;
// IntTable inttable;
// StrTable stringtable;

class Constants {
private:
	Constants(IdTable *idTable);
public:
	Symbol arg, 
	arg2,
	Bool,
	concat,
	cool_abort,
	copy,
	Int,
	in_int,
	in_string,
	IO,
	length,
	Main,
	main_meth,
	//   _no_class is a symbol that can't be the name of any 
	//   user-defined class.
	No_class,
	No_type,
	Object,
	out_int,
	out_string,
	prim_slot,
	self,
	SELF_TYPE,
	Str,
	str_field,
	substr,
	type_name,
	val,
	constructor;
	
	static Constants & getInstance(IdTable * idtable);
};

class GlobalTables {
private:
	
	Constants * constants;
	
	GlobalTables();
	
	GlobalTables(GlobalTables const&);   // Not implemented.
	void operator=(GlobalTables const&); // Not implemented
public:
	
	IdTable idtable;
	IntTable inttable;
	StrTable stringtable;
	
	static GlobalTables & getInstance();
	
	Constants & get_constants();
};

#endif
