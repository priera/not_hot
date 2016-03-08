#include "emitter.h"

using namespace cgen;
using namespace cgen::constants;

void Emitter::load(std::string dest_reg, int offset, std::string source_reg, ostream & s)
{
	s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
	<< endl;
}

void Emitter::store(std::string source_reg, int offset, std::string dest_reg, ostream & s)
{
	s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
	<< endl;
}

void Emitter::load_imm(std::string dest_reg, int val, ostream & s)
{ s << LI << dest_reg << " " << val << endl; }

void Emitter::load_address(std::string dest_reg, std::string address, ostream & s)
{ s << LA << dest_reg << " " << address << endl; }

void Emitter::partial_load_address(std::string dest_reg, ostream & s)
{ s << LA << dest_reg << " "; }

void Emitter::load_bool(std::string dest, const symbol::BoolConst& b, ostream & s)
{
	partial_load_address(dest, s);
	b.code_ref(s);
	s << endl;
}

void Emitter::load_bool_val(std::string dest, const symbol::BoolConst& b, ostream & s){
	load_bool(dest, b, s);
	load(dest, 3, dest, s);
}

void Emitter::load_string(std::string dest, const symbol::CgenStringEntry & str, ostream & s)
{
	partial_load_address(dest, s);
	str.code_ref(s);
	s << endl;
}

void Emitter::load_int(std::string dest, const symbol::CgenIntEntry & i, ostream & s)
{
	partial_load_address(dest, s);
	i.code_ref(s);
	s << endl;
}

void Emitter::default_value_ref(Symbol type, ostream & s){
	using namespace ::cgen::symbol;
	
	::Constants c = GlobalTables::getInstance().get_constants();
	
	if (type == c.Int) {
		CgenIntEntry::empty().code_ref(s);
	} else if ( type == c.Str){
		CgenStringEntry::empty().code_ref(s);
	} else if (type == c.Bool) {
		BoolConst::false_().code_ref(s);
	} else {
		s << "0";
	} 
	
	s << endl;
}

void Emitter::test_basic_equality(ostream & s){
	using namespace ::cgen::symbol;
	
	load_bool(ACC,BoolConst::true_(),s);
	load_bool(A1,BoolConst::false_(),s);
	
	jal("equality_test",s);
	
	load_bool_val(T1,BoolConst::true_(),s);
	load(ACC,3,ACC,s);
	
	int equal = LabelMgr::getInstance().nextLabel();
	int done = LabelMgr::getInstance().nextLabel();
	
	beq(ACC,T1,equal,s);
	load_bool(ACC,BoolConst::false_(),s);
	branch(done,s);
	label_def(equal,s);
	load_bool(ACC,BoolConst::true_(),s);
	label_def(done,s);
}

void Emitter::move(std::string dest_reg, std::string source_reg, ostream & s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

void Emitter::neg(std::string dest, std::string src1, ostream & s)
{ s << NEG << dest << " " << src1 << endl; }

void Emitter::add(std::string dest, std::string src1, std::string src2, ostream & s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

void Emitter::addu(std::string dest, std::string src1, std::string src2, ostream & s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

void Emitter::addiu(std::string dest, std::string src1, int imm, ostream & s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

void Emitter::div(std::string dest, std::string src1, std::string src2, ostream & s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

void Emitter::mul(std::string dest, std::string src1, std::string src2, ostream & s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

void Emitter::sub(std::string dest, std::string src1, std::string src2, ostream & s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

void Emitter::sll(std::string dest, std::string src1, int num, ostream & s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

void Emitter::jalr(std::string dest, ostream & s)
{ s << JALR << "\t" << dest << endl; }

void Emitter::jr(std::string dest, ostream & s)
{ s << JR << "\t" << dest << endl; }

void Emitter::jal(std::string address, ostream & s)
{ s << JAL << address << endl; }

void Emitter::return_(ostream & s)
{ s << RET << endl; }

void Emitter::gc_assign(ostream & s)
{ s << JAL << "_GenGC_Assign" << endl; }

void Emitter::dispatch_table_ref(Symbol sym, ostream & s)
{  s << sym << DISPTAB_SUFFIX; }

void Emitter::init_ref(Symbol sym, ostream & s)
{ s << sym << CLASSINIT_SUFFIX; }

void Emitter::label_ref(int l, ostream & s)
{ s << "label" << l; }

void Emitter::protobj_ref(Symbol sym, ostream & s)
{ s << sym << PROTOBJ_SUFFIX; }

void Emitter::dispath_table_ref(Symbol sym, ostream & s)
{	s << sym << DISPTAB_SUFFIX; }

void Emitter::method_ref(Symbol classname, Symbol methodname, ostream & s)
{ s << classname << METHOD_SEP << methodname; }

void Emitter::label_def(int l, ostream & s)
{
	label_ref(l, s);
	s << ":" << endl;
}

void Emitter::beqz(std::string source, int label, ostream & s)
{
	s << BEQZ << source << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::beq(std::string src1, std::string src2, int label, ostream & s)
{
	s << BEQ << src1 << " " << src2 << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::bne(std::string src1, std::string src2, int label, ostream & s)
{
	s << BNE << src1 << " " << src2 << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::bleq(std::string src1, std::string src2, int label, ostream & s)
{
	s << BLEQ << src1 << " " << src2 << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::blt(std::string src1, std::string src2, int label, ostream & s)
{
	s << BLT << src1 << " " << src2 << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::blti(std::string src1, int imm, int label, ostream & s)
{
	s << BLT << src1 << " " << imm << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::bgti(std::string src1, int imm, int label, ostream & s)
{
	s << BGT << src1 << " " << imm << " ";
	label_ref(label, s);
	s << endl;
}

void Emitter::branch(int l, ostream & s)
{
	s << BRANCH;
	label_ref(l, s);
	s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
void Emitter::push(std::string reg, ostream & s)
{
	store(reg,0,SP,s);
	addiu(SP,SP,-4,s);
}

void Emitter::pop(std::string dest_reg, ostream & s){
	load(dest_reg, 1, SP, s);
	addiu(SP,SP,4,s);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
void Emitter::fetch_int(std::string dest, std::string source, ostream & s)
{ load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
void Emitter::store_int(std::string source, std::string dest, ostream & s)
{ store(source, DEFAULT_OBJFIELDS, dest, s); }

void Emitter::callee_begin(int locals_count, ostream & s){
	
	//set new FP, and return address
	move(FP,SP,s);
	int bytes = -(locals_count * WORD_SIZE);
	addiu(SP,SP,bytes,s);
	push(RA,s);

}

void Emitter::callee_return(int arg_count, int locals_count, ostream & s){
	
	//popping ACC, RA, function arguments, and locals reserved space
	load(RA, 1, SP, s);
	int bytes = ((arg_count + locals_count) * WORD_SIZE) + WORD_SIZE;
	addiu(SP,SP,bytes,s);
	
	/*//restores SELF, FP, and return
	pop(SELF,s);
	pop(FP,s);*/
	return_(s);
}

void Emitter::new_(Symbol class_name, ostream & s){
	partial_load_address(ACC, s); Emitter::protobj_ref(class_name,s);
	s << endl;
	
	Emitter::jal("Object.copy",s);
	push(ACC,s); //Since the initialization can override the pointer to the new object, it is saved

	push(FP,s);
	push(SELF,s);
	move(SELF,ACC,s);
	
	jal(std::string(class_name->get_string()) + CLASSINIT_SUFFIX,s);
	
	pop(SELF,s);
	pop(FP,s);
}

void Emitter::new_int(std::string reg_name, ostream & s){
	push(reg_name,s);
	new_(GlobalTables::getInstance().get_constants().Int, s);	
	pop(ACC,s);
	pop(reg_name,s);
	store_int(reg_name,ACC,s);
}

/*void Emitter::test_collector()
{
//		push(ACC);
// 	move(ACC, SP); // stack end
// 	move(A1, ZERO); // allocate nothing
// 	s << JAL << gc_collect_names[cgen_Memmgr] << endl;
// 	addiu(SP,SP,4);
// 	load(ACC,0,SP);
}*/

void Emitter::gc_check(std::string source, ostream & s)
{
	if (source.compare(A1) == 0) move(A1, source, s);
	s << JAL << "_gc_check" << endl;
}

void Emitter::case_branch_ref(int case_index, int branch_index, ostream & s){
	s << "case_" << case_index << "_branch_" << branch_index;
}

void Emitter::case_end_ref(int case_index, ostream & s){
	s << "end_case_" << case_index;
}

void Emitter::case_table_ref(int case_index, ostream & s){
	s << "case_" << case_index;
}

void Emitter::jump_to_case_end(int case_index, ostream & s){
	s << BRANCH;
	case_end_ref(case_index, s);
	s << "\n";
}

void Emitter::branch_resolver(ostream & s){
	ifstream myfile("./as/branch_resolver.s"); //TODO: resoldre el path
	string line;
	if (myfile.is_open())
	{		
		while ( myfile.good() )
		{
			getline(myfile,line);
			s << line << endl;
		}
		myfile.close();
	}
}

void Emitter::jump_to_branch_resolver(int case_index, ostream &s){
	ostringstream ss;
	ss << case_index;
	
	load_address(T1,std::string("case_") + ss.str() ,s);
	load_address(T2,CLASS_COUNT_TAG,s);
	load(T2,0,T2,s);
	s << JAL << "_branch_resolver" << "\n";
}

void Emitter::jump_to_proper_branch(ostream & s){
	load_imm(T2,2*WORD_SIZE,s);
	mul(T0,T0,T2,s);
	add(T1,T1,T0,s);
	addiu(T1,T1,WORD_SIZE,s);
	load(T1,0,T1,s);
	jr(T1,s);
}

void Emitter::check_dispatch_on_void(int not_void_label, int lineno, symbol::CgenStringEntry & entry, ostream & s){
	Emitter::load_imm(T1,0,s);
	Emitter::bne(ACC,T1,not_void_label,s);
	Emitter::load_imm(T1,lineno,s);
	Emitter::partial_load_address(ACC,s);
	entry.code_ref(s); s << endl;
	Emitter::jal(DISPATCH_VOID_HANDLER, s);
	
	Emitter::label_def(not_void_label,s);
}

void Emitter::check_case_on_void(int not_void_label, int lineno, symbol::CgenStringEntry & entry, ostream & s){
	Emitter::load_imm(T1,0,s);
	Emitter::bne(ACC,T1,not_void_label,s);
	Emitter::load_imm(T1,lineno,s);
	Emitter::partial_load_address(ACC,s);
	entry.code_ref(s); s << endl;
	Emitter::jal(CASE_VOID_HANDLER, s);
	
	Emitter::label_def(not_void_label,s);
}

int Emitter::ascii = 0;

void Emitter::ascii_mode(ostream& str)
{
	if (!ascii) 
	{
		str << "\t.ascii\t\"";
		ascii = 1;
	} 
}

void Emitter::byte_mode(ostream& str)
{
	if (ascii) 
	{
		str << "\"\n";
		ascii = 0;
	}
}

 void Emitter::string_constant(char const * s, ostream& str)
{
	ascii = 0;
	
	while (*s) {
		switch (*s) {
			case '\n':
				ascii_mode(str);
				str << "\\n";
				break;
			case '\t':
				ascii_mode(str);
				str << "\\t";
				break;
			case '\\':
				byte_mode(str);
				str << "\t.byte\t" << (int) ((unsigned char) '\\') << endl;
				break;
			case '"' :
				ascii_mode(str);
				str << "\\\"";
				break;
			default:
				if (*s >= ' ' && ((unsigned char) *s) < 128) {
					ascii_mode(str);
					str << *s;
				} else {
					byte_mode(str);
					str << "\t.byte\t" << (int) ((unsigned char) *s) << endl;
				}
				break;
		}
		s++;
	}
	byte_mode(str);
	str << "\t.byte\t0\t" << endl;
}

