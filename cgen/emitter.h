#ifndef EMIT_H
#define EMIT_H

#include "cgen_symbol.h"
#include "constants.h"

using namespace std;

namespace cgen {
	
	namespace constants {
		const int MAXINT  = 100000000;
		const int WORD_SIZE = 4;
		const int LOG_WORD_SIZE = 2;     // for logical shifts
		
		// Global names
		const string CLASSNAMETAB = "class_nameTab";
		const string CLASSOBJTAB = "class_objTab";
		const string INTTAG = "_int_tag";
		const string BOOLTAG = "_bool_tag";
		const string STRINGTAG = "_string_tag";
		const string HEAP_START = "heap_start";
		const string DISPATCH_VOID_HANDLER = "_dispatch_abort";
		
		// Naming conventions
		const string DISPTAB_SUFFIX = "_dispTab";
		const string METHOD_SEP = ".";
		const string CLASSINIT_SUFFIX = "_init";
		const string PROTOBJ_SUFFIX = "_protObj";
		const string OBJECTPROTOBJ = "Object_protObj";
		const string INTCONST_PREFIX = "int_const";
		const string STRCONST_PREFIX = "str_const";
		const string BOOLCONST_PREFIX = "bool_const";
		
		
		const int EMPTYSLOT = 0;
		const string LABEL = ":\n";
		
		const string STRINGNAME = "String";
		const string INTNAME = "Int";
		const string BOOLNAME  = "Bool";
		const string MAINNAME = "Main";
		
		//
		// information about object headers
		//
		const int DEFAULT_OBJFIELDS = 3;
		const int TAG_OFFSET = 0;
		const int SIZE_OFFSET = 1;
		const int DISPTABLE_OFFSET = 2;
		
		const int STRING_SLOTS = 1;
		const int INT_SLOTS = 1;
		const int BOOL_SLOTS = 1;
		
		const string GLOBAL = "\t.globl\t";
		const string ALIGN = "\t.align\t2\n";
		const string WORD  = "\t.word\t";
		
		//
		// register names
		//
		const string ZERO = "$zero";		// Zero register 
		const string ACC = "$a0";		// Accumulator 
		const string A1 = "$a1";		// For arguments to prim funcs 
		const string SELF = "$s0";		// Ptr to self (callee saves) 
		const string T1 = "$t1";		// Temporary 1 
		const string T2 = "$t2";		// Temporary 2 
		const string T3 = "$t3";		// Temporary 3 
		const string SP = "$sp";		// Stack pointer 
		const string FP = "$fp";		// Frame pointer 
		const string RA = "$ra";		// Return address 
		
		//
		// Opcodes
		//
		const string JALR = "\tjalr\t";  
		const string JAL = "\tjal\t";                 
		const string RET = "\tjr\t" + RA + "\t";
		
		const string SW = "\tsw\t";
		const string LW = "\tlw\t";
		const string LI =  "\tli\t";
		const string LA =  "\tla\t";
		
		const string MOVE = "\tmove\t";
		const string NEG =  "\tneg\t";
		const string ADD =  "\tadd\t";
		const string ADDI = "\taddi\t";
		const string ADDU = "\taddu\t";
		const string ADDIU = "\taddiu\t";
		const string DIV = "\tdiv\t";
		const string MUL = "\tmul\t";
		const string SUB = "\tsub\t";
		const string SLL = "\tsll\t";
		const string BEQZ = "\tbeqz\t";
		const string BRANCH = "\tb\t";
		const string BEQ ="\tbeq\t";
		const string BNE = "\tbne\t";
		const string BLEQ = "\tble\t";
		const string BLT = "\tblt\t";
		const string BGT = "\tbgt\t";
	}
	
	class Emitter {
	private:
		static void ascii_mode(ostream& str);
		static void byte_mode(ostream& str);
		static int ascii;
		
	public:
		
		static void load(std::string dest_reg, int offset, std::string source_reg , ostream & s);
		
 		static void store(std::string source_reg, int offset, std::string dest_reg , ostream & s);
		
		static void load_imm(std::string dest_reg, int val , ostream & s);
		
		static void load_address(std::string dest_reg, std::string address, ostream & s);;
		
		static void partial_load_address(std::string dest_reg, ostream & s);
		
		static void load_bool(std::string dest, const symbol::BoolConst& b, ostream & s);
		
		static void load_bool_val(std::string dest, const symbol::BoolConst& b, ostream & s);
		
		static void load_string(std::string dest, const symbol::CgenStringEntry & str, ostream & s);
		
		static void load_int(std::string dest, const symbol::CgenIntEntry & i, ostream & s);
		
		static void default_value_ref(Symbol type, ostream & s);
		
		static void test_basic_equality(ostream & s);
		
		static void move(std::string dest_reg, std::string source_reg, ostream & s);
		
		static void neg(std::string dest, std::string src1, ostream & s);
		
		static void add(std::string dest, std::string src1, std::string src2, ostream & s);
		
		static void addu(std::string dest, std::string src1, std::string src2, ostream & s);

		static void addiu(std::string dest, std::string src1, int imm, ostream & s);
		
		static void div(std::string dest, std::string src1, std::string src2, ostream & s);
		
		static void mul(std::string dest, std::string src1, std::string src2, ostream & s);
		
		static void sub(std::string dest, std::string src1, std::string src2, ostream & s);
		
		static void sll(std::string dest, std::string src1, int num, ostream & s);
		
		static void jalr(std::string dest, ostream & s);
		
		static void jal(std::string address, ostream & s);
		
		static void return_(ostream & s);
		
		static void gc_assign(ostream & s);
		
		static void dispatch_table_ref(Symbol sym, ostream & s);
		
		static void init_ref(Symbol sym, ostream & s);
		
		static void label_ref(int l , ostream & s);
		
		static void protobj_ref(Symbol sym, ostream & s);
		
		static void dispath_table_ref(Symbol sym, ostream & s);
		
		static void method_ref(Symbol classname, Symbol methodname, ostream & s);
		
		static void label_def(int l, ostream & s);
		
		static void beqz(std::string source, int label, ostream & s);
		
		static void beq(std::string src1, std::string src2, int label, ostream & s);
		
		static void bne(std::string src1, std::string src2, int label, ostream & s);
		
		static void bleq(std::string src1, std::string src2, int label, ostream & s);
		
		static void blt(std::string src1, std::string src2, int label, ostream & s);
		
		static void blti(std::string src1, int imm, int label, ostream & s);
		
		static void bgti(std::string src1, int imm, int label, ostream & s);
		
		static void branch(int l, ostream & s);
		
		static void callee_begin(int locals_count, ostream & s);
		
		static void callee_return(int arg_count, int locals_count, ostream & s);
		
		static void new_(Symbol class_name, ostream & s);
		
		static void new_int(std::string reg_name, ostream & s);
		
		//
		// Push a register on the stack. The stack grows towards smaller addresses.
		//
 		static void push(std::string reg, ostream & s);
		
		static void pop(std::string dest_reg, ostream & s);
		
		//
		// Fetch the integer value in an Int object.
		// Emits code to fetch the integer value of the Integer object pointed
		// to by register source into the register dest
		//
		static void fetch_int(std::string dest, std::string source, ostream & s);
		
		//
		// Emits code to store the integer value contained in register source
		// into the Integer object pointed to by dest.
		//
		static void store_int(std::string source, std::string dest, ostream & s);
		
		//static void test_collector(, ostream & s);
		
		static void gc_check(std::string source, ostream & s);
		
		static void string_constant(char* s, ostream & str);
		
		static void check_dispatch_on_void(int not_void_label, int lineno, symbol::CgenStringEntry & entry, ostream & s);
	};
	
} 

#endif