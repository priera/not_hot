#include "stringtab.h"

#include "cool-io.h"

#define MAXSIZE 1000000
#define min(a,b) (a > b ? b : a)

//
// A string table is implemented a linked list of Entrys.  Each Entry
// in the list has a unique string.
//

template <class Elem>
Elem *StringTable<Elem>::add_string(char *s)
{
	return add_string(s,MAXSIZE);
}

//
// Add a string requires two steps.  First, the list is searched; if the
// string is found, a pointer to the existing Entry for that string is 
// returned.  If the string is not found, a new Entry is created and added
// to the list.
//
template <class Elem>
Elem *StringTable<Elem>::add_string(char *s, int maxchars)
{
	int len = min((int) strlen(s),maxchars);
	for(List<Elem> *l = tbl; l; l = l->tl()){
		if (l->hd()->equal_string(s,len)){
			return l->hd();
		}
	}
	
	Elem *e = new Elem(s,len,index++);
	tbl = new List<Elem>(e, tbl);
	return e;
}

//
// To look up a string, the list is scanned until a matching Entry is located.
// If no such entry is found, an assertion failure occurs.  Thus, this function
// is used only for strings that one expects to find in the table.
//
template <class Elem>
Elem *StringTable<Elem>::lookup_string(char *s)
{
	int len = strlen(s);
	for(List<Elem> *l = tbl; l; l = l->tl())
		if (l->hd()->equal_string(s,len))
			return l->hd();
		assert(0);   // fail if string is not found
		return NULL; // to avoid compiler warning
}

//
// lookup is similar to lookup_string, but uses the index of the string
// as the key.
//
template <class Elem>
Elem *StringTable<Elem>::lookup(int ind)
{
	for(List<Elem> *l = tbl; l; l = l->tl())
		if (l->hd()->equal_index(ind))
			return l->hd();
		assert(0);   // fail if string is not found
		return NULL; // to avoid compiler warning
}

//
// add_int adds the string representation of an integer to the list.
//
template <class Elem>
Elem *StringTable<Elem>::add_int(int i)
{
	static char *buf = new char[20];
	snprintf(buf, 20, "%d", i);
	return add_string(buf);
}
template <class Elem>
int StringTable<Elem>::first()
{
	return 0;
}

template <class Elem>
int StringTable<Elem>::more(int i)
{
	return i < index;
}

template <class Elem>
int StringTable<Elem>::next(int i)
{
	assert(i < index);
	return i+1;
}

template <class Elem>
void StringTable<Elem>::print()
{
	list_print(cerr,tbl);
}


StringEntry * StrTable::add_string(char *s){
	GlobalTables::getInstance().idtable.add_int(strlen(s));
	return StringTable<StringEntry>::add_string(s);
}

//
// Explicit template instantiations.
// Comment out for versions of g++ prior to 2.7
//TODO: Per qué conyo serveix això???
//
template class StringTable<IdEntry>;
template class StringTable<StringEntry>;
template class StringTable<IntEntry>;

Constants::Constants(IdTable * idtable){
	arg         = idtable->add_string("arg");
	arg2        = idtable->add_string("arg2");
	Bool        = idtable->add_string("Bool");
	concat      = idtable->add_string("concat");
	cool_abort  = idtable->add_string("abort");
	copy        = idtable->add_string("copy");
	Int         = idtable->add_string("Int");
	in_int      = idtable->add_string("in_int");
	in_string   = idtable->add_string("in_string");
	IO          = idtable->add_string("IO");
	length      = idtable->add_string("length");
	Main        = idtable->add_string("Main");
	main_meth   = idtable->add_string("main");
	//   _no_class is a symbol that can't be the name of any 
	//   user-defined class.
	No_class    = idtable->add_string("_no_class");
	No_type     = idtable->add_string("_no_type");
	Object      = idtable->add_string("Object");
	out_int     = idtable->add_string("out_int");
	out_string  = idtable->add_string("out_string");
	prim_slot   = idtable->add_string("_prim_slot");
	self        = idtable->add_string("self");
	SELF_TYPE   = idtable->add_string("SELF_TYPE");
	Str         = idtable->add_string("String");
	str_field   = idtable->add_string("_str_field");
	substr      = idtable->add_string("substr");
	type_name   = idtable->add_string("type_name");
	val         = idtable->add_string("_val");
	
	constructor = idtable->add_string("_constructor");
}

Constants & Constants::getInstance(IdTable * idtable){
	static Constants c(idtable);
	return c;
}

GlobalTables::GlobalTables() {
	constants = &Constants::getInstance(&idtable);
}

GlobalTables & GlobalTables::getInstance(){
	static GlobalTables tables;
	static bool init = false;
	if (!init) {
		init = true;
		tables.stringtable.add_string("");
	}
	return tables;
}

Constants & GlobalTables::get_constants(){
	return *constants;
}