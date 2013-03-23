#ifndef EXPRESSION_CODER_H
#define EXPRESSION_CODER_H

#include "stringtab.h"
#include "cool-tree.h"
#include "emitter.h"
#include "constants.h"
#include "cgen_symbol.h"
#include "cgen_node.h"
#include "cgen_tables.h"

namespace cgen{
	
	class ExpressionCoder {
	private:
		Expression expr_;
		
		void code_assign(ostream &s);
		void code_static_dispatch(ostream &s);
		void code_dispatch(ostream &s);
		void code_cond(ostream &s);
		void code_loop(ostream &s);
		void code_typcase(ostream &s);
		void code_block(ostream &s);
		void code_let(ostream &s);
		void code_plus(ostream &s);
		void code_sub(ostream &s);
		void code_mul(ostream &s);
		void code_divide(ostream &s);
		void code_neg(ostream &s);
		void code_lt(ostream &s);
		void code_eq(ostream &s);
		void code_leq(ostream &s);
		void code_comp(ostream &s);
		void code_int_const(ostream &s);
		void code_bool_const(ostream &s);
		void code_string_const(ostream &s);
		void code_new(ostream &s);
		void code_isvoid(ostream &s);
		void code_no_expr(ostream &s);
		void code_obj(ostream &s);
		
		int count_locals_in_assign();
		int count_locals_in_static_dispatch();
		int count_locals_in_dispatch();
		int count_locals_in_cond();
		int count_locals_in_loop();
		int count_locals_in_typcase();
		int count_locals_in_block();
		int count_locals_in_let();
		int count_locals_in_neg();
		int count_locals_in_eq();
		int count_locals_in_comp();
		int count_locals_in_new();
		int count_locals_in_isvoid();
		int count_locals_in_binary_arithmetic();
		int count_locals_in_binary_relational();

	public:
		ExpressionCoder(Expression);
	
		void code(ostream &);
		int countLocals();
	};

}
#endif
