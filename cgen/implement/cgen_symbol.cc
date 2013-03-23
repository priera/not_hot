#include "cgen_symbol.h"

using namespace cgen::constants;
using namespace cgen::symbol;

CgenStringEntry::CgenStringEntry(StringEntry & entry) : StringEntry(entry) { }

void CgenStringEntry::code_ref(ostream& s) const
{
	s << STRCONST_PREFIX << index;
}

void CgenStringEntry::code_def(ostream& s, int stringclasstag)
{
	
	IntEntry * lensym = GlobalTables::getInstance().inttable.add_int(len);
	// Add -1 eye catcher
	s << WORD << "-1" << endl;
	
	code_ref(s);  s  << LABEL                                             // label
	<< WORD << stringclasstag << endl                                 // tag
	<< WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
	<< WORD << STRINGNAME << DISPTAB_SUFFIX << endl;        // dispatch table
	s << WORD;  CgenIntEntry(*lensym).code_ref(s);  s << endl;            // string length
	Emitter::string_constant(str,s);                                // ascii string
	s << ALIGN;                                                 // align to word
}

CgenStringEntry & CgenStringEntry::empty() {
	static StringEntry * entry = GlobalTables::getInstance().stringtable.add_string("");
	static CgenStringEntry val = CgenStringEntry(*entry);
	return val;
}

CgenIntEntry::CgenIntEntry(IntEntry & entry) : IntEntry(entry) {}

void CgenIntEntry::code_ref(ostream& s) const {
	s << INTCONST_PREFIX << index;
}

void CgenIntEntry::code_def(ostream& s, int intclasstag) 
{
	// Add -1 eye catcher
	s << WORD << "-1" << endl;
	
	code_ref(s);  s << LABEL                                // label
	<< WORD << intclasstag << endl                      // class tag
	<< WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
	<< WORD << INTNAME << DISPTAB_SUFFIX << endl;       // dispatch table
	s << WORD << str << endl;                           // integer value

}

CgenIntEntry & CgenIntEntry::empty() {
	static IntEntry * entry = GlobalTables::getInstance().inttable.add_int(0);
	static CgenIntEntry val = CgenIntEntry(*entry);
	return val;
}

BoolConst::BoolConst(int i) : val(i) {assert( i == 0 || i == 1);}

BoolConst & BoolConst::true_() {
	static BoolConst t(1);
	return t;
}

BoolConst & BoolConst::false_(){
	static BoolConst t(0);
	return t;
}

void BoolConst::code_ref(ostream& s) const
{
	s << BOOLCONST_PREFIX << val;
}

//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
	// Add -1 eye catcher
	s << WORD << "-1" << endl;
	
	code_ref(s);  s << LABEL                                  // label
	<< WORD << boolclasstag << endl                       // class tag
	<< WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
	<< WORD << BOOLNAME << DISPTAB_SUFFIX << endl;        // dispatch table
	s << WORD << val << endl;                             // value (0 or 1)
}

BoolConst & BoolConst::getProperBool(bool val){
	return (val) ? true_() : false_() ;
}
