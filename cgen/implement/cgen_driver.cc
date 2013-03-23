#include "cgen_driver.h"

using namespace cgen;

Driver::Driver(ostream & output) : output_(output), \
baseNodeClass_(new CgenNode(*((class__class *)ClassTable::getInstance().get_base_class()), NULL)) {
	
}


char * Driver::getCharPointer(string str){
	char * ca = new char[str.size()+1];
	std::copy(str.begin(), str.end(), ca);
	ca[str.size()] = '\0';
	return ca;
}

void Driver::codeGlobalData()
{
	using namespace constants;
	
	Symbol main    = GlobalTables::getInstance().idtable.lookup_string(getCharPointer(MAINNAME));
	Symbol string  = GlobalTables::getInstance().idtable.lookup_string(getCharPointer(STRINGNAME));
	Symbol integer = GlobalTables::getInstance().idtable.lookup_string(getCharPointer(INTNAME));
	Symbol boolc   = GlobalTables::getInstance().idtable.lookup_string(getCharPointer(BOOLNAME));
	
	output_ << "\t.data\n" << ALIGN;
	//
	// The following global names must be defined first.
	//
	output_ << GLOBAL << CLASSNAMETAB << endl;
	output_ << GLOBAL; Emitter::protobj_ref(main, output_);    output_ << endl;
	output_ << GLOBAL; Emitter::protobj_ref(integer, output_); output_ << endl;
	output_ << GLOBAL; Emitter::protobj_ref(string, output_);  output_ << endl;
	output_ << GLOBAL; symbol::BoolConst::false_().code_ref(output_);  output_ << endl;
	output_ << GLOBAL; symbol::BoolConst::true_().code_ref(output_);   output_ << endl;
	output_ << GLOBAL << INTTAG << endl;
	output_ << GLOBAL << BOOLTAG << endl;
	output_ << GLOBAL << STRINGTAG << endl;
	
	output_ << GLOBAL; Emitter::dispatch_table_ref(main, output_); output_ << endl;
	output_ << GLOBAL; Emitter::dispatch_table_ref(string, output_); output_ << endl;
	output_ << GLOBAL; Emitter::dispatch_table_ref(integer, output_); output_ << endl;
	output_ << GLOBAL; Emitter::dispatch_table_ref(boolc, output_); output_ << endl;
	//
	// We also need to know the tag of the Int, String, and Bool classes
	// during code generation.
	//
 	output_ << INTTAG << LABEL
 	<< WORD << Constants::getInstance().get_base_class_tag(Constants::INT) << endl;
 	output_ << BOOLTAG << LABEL 
 	<< WORD << Constants::getInstance().get_base_class_tag(Constants::BOOL) << endl;
	output_ << STRINGTAG << LABEL 
	<< WORD << Constants::getInstance().get_base_class_tag(Constants::STR) << endl;   
}

void Driver::codeBools(){
	
	using namespace cgen::symbol;
	int boolclasstag = Constants::getInstance().get_base_class_tag(Constants::BOOL);
	
	BoolConst::true_().code_def(output_, boolclasstag);
	BoolConst::false_().code_def(output_, boolclasstag);
}

// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table, and their length should be in the integer table
// The constants are emmitted by running through the stringtable and inttable
// and producing code for each entry.
void Driver::codeConstantTables() {

	int stringclasstag = Constants::getInstance().get_base_class_tag(Constants::STR);
	int intclasstag = Constants::getInstance().get_base_class_tag(Constants::INT);
	
	CodifierStrTable strTab = CodifierStrTable(GlobalTables::getInstance().stringtable, stringclasstag);
	strTab.code(output_);
	
	CodifierIntTable intTab = CodifierIntTable(GlobalTables::getInstance().inttable, intclasstag);
	intTab.code(output_);
	
	codeBools();
}

void Driver::emitClassNameTab(){
	using namespace cgen::constants;
	using namespace cgen::symbol;
	
	output_ << CLASSNAMETAB << LABEL;
	
	BasicListIterator<CgenNode> * iter = CgenNodesTable::getInstance().getIterator();
	Symbol s;
	StringEntryP temp = NULL;
	
	while (iter->more()){
		s = iter->current()->get_name();
		temp = GlobalTables::getInstance().stringtable.lookup_string(s->get_string());
		output_ << WORD; CgenStringEntry(*temp).code_ref(output_);
		output_ << endl;
		iter->move();
	}

}

void Driver::codeGlobalText() {
	using namespace cgen::constants;
	
	output_ << GLOBAL << HEAP_START << endl
	<< HEAP_START << LABEL 
	<< WORD << 0 << endl
	<< "\t.text" << endl
	<< GLOBAL;
	
	::Constants c = GlobalTables::getInstance().get_constants();
	
	Emitter::init_ref(c.Main, output_);
	output_ << endl << GLOBAL;
	Emitter::init_ref(c.Int,output_);
	output_ << endl << GLOBAL;
	Emitter::init_ref(c.Str,output_);
	output_ << endl << GLOBAL;
	Emitter::init_ref(c.Bool,output_);
	output_ << endl << GLOBAL;
	Emitter::method_ref(c.Main, c.main_meth, output_);
	output_ << endl;
}

void Driver::codeSelectGc() {
	//
	// Generate GC choice constants (pointers to GC functions)
	//
	
	using namespace cgen::constants;
	using namespace ::flags;
	
	static char *gc_init_names[] =
	{ "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
	static char *gc_collect_names[] =
	{ "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };
	
	output_ << GLOBAL << "_MemMgr_INITIALIZER" << endl;
	output_ << "_MemMgr_INITIALIZER:" << endl;
	output_ << WORD << gc_init_names[GarbageCollectionOptions::GC_NOGC] << endl;
	output_ << GLOBAL << "_MemMgr_COLLECTOR" << endl;
	output_ << "_MemMgr_COLLECTOR:" << endl;
	output_ << WORD << gc_collect_names[GarbageCollectionOptions::GC_NOGC] << endl;
	output_ << GLOBAL << "_MemMgr_TEST" << endl;
	output_ << "_MemMgr_TEST:" << endl;
	output_ << WORD << (GarbageCollectionOptions::GC_NORMAL == GarbageCollectionOptions::GC_TEST) << endl;
}

void Driver::code(){
	
	codeGlobalData();
	
	codeSelectGc();
	
	//Coding constant information
	codeConstantTables();
	baseNodeClass_->emitPrototables(output_);
	baseNodeClass_->emitDispatchTable(output_);
	emitClassNameTab();

	//Coding program's code
 	codeGlobalText();	
	baseNodeClass_->code(output_);
	
}