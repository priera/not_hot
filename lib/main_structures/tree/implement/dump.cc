#include "cool-tree.h"
#include <basic/cool.h>

// constructors' functions
Program program_class::copy_Program()
{
	return new program_class(classes->copy_list());
}


void program_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "program\n";
	classes->dump(stream, n+2);
}

Class_ class__class::copy_Class_()
{
	return new class__class(copy_Symbol(name), copy_Symbol(parent), features->copy_list(), copy_Symbol(filename));
}


void class__class::dump(ostream& stream, int n)
{
	stream << pad(n) << "class_\n";
	dump_Symbol(stream, n+2, name);
	dump_Symbol(stream, n+2, parent);
	features->dump(stream, n+2);
	dump_Symbol(stream, n+2, filename);
}

Feature method_class::copy_Feature()
{
	return new method_class(copy_Symbol(name), formals->copy_list(), copy_Symbol(return_type), expr->copy_Expression());
}


void method_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "method\n";
	dump_Symbol(stream, n+2, name);
	formals->dump(stream, n+2);
	dump_Symbol(stream, n+2, return_type);
	expr->dump(stream, n+2);
}

Feature attr_class::copy_Feature()
{
	return new attr_class(copy_Symbol(name), copy_Symbol(type_decl), init->copy_Expression());
}


void attr_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "attr\n";
	dump_Symbol(stream, n+2, name);
	dump_Symbol(stream, n+2, type_decl);
	init->dump(stream, n+2);
}


Formal formal_class::copy_Formal()
{
	return new formal_class(copy_Symbol(name), copy_Symbol(type_decl));
}


void formal_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "formal\n";
	dump_Symbol(stream, n+2, name);
	dump_Symbol(stream, n+2, type_decl);
}

Case branch_class::copy_Case()
{
	return new branch_class(copy_Symbol(name), copy_Symbol(type_decl), expr->copy_Expression());
}


void branch_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "branch\n";
	dump_Symbol(stream, n+2, name);
	dump_Symbol(stream, n+2, type_decl);
	expr->dump(stream, n+2);
}


Expression assign_class::copy_Expression()
{
	return new assign_class(copy_Symbol(name), expr->copy_Expression());
}


void assign_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "assign\n";
	dump_Symbol(stream, n+2, name);
	expr->dump(stream, n+2);
}


Expression static_dispatch_class::copy_Expression()
{
	return new static_dispatch_class(expr->copy_Expression(), copy_Symbol(type_name), copy_Symbol(name), actual->copy_list());
}


void static_dispatch_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "static_dispatch\n";
	expr->dump(stream, n+2);
	dump_Symbol(stream, n+2, type_name);
	dump_Symbol(stream, n+2, name);
	actual->dump(stream, n+2);
}


Expression dispatch_class::copy_Expression()
{
	return new dispatch_class(expr->copy_Expression(), copy_Symbol(name), actual->copy_list());
}


void dispatch_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "dispatch\n";
	expr->dump(stream, n+2);
	dump_Symbol(stream, n+2, name);
	actual->dump(stream, n+2);
}


Expression cond_class::copy_Expression()
{
	return new cond_class(pred->copy_Expression(), then_exp->copy_Expression(), else_exp->copy_Expression());
}


void cond_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "cond\n";
	pred->dump(stream, n+2);
	then_exp->dump(stream, n+2);
	else_exp->dump(stream, n+2);
}


Expression loop_class::copy_Expression()
{
	return new loop_class(pred->copy_Expression(), body->copy_Expression());
}


void loop_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "loop\n";
	pred->dump(stream, n+2);
	body->dump(stream, n+2);
}


Expression typcase_class::copy_Expression()
{
	return new typcase_class(expr->copy_Expression(), cases->copy_list());
}


void typcase_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "typcase\n";
	expr->dump(stream, n+2);
	cases->dump(stream, n+2);
}


Expression block_class::copy_Expression()
{
	return new block_class(body->copy_list());
}


void block_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "block\n";
	body->dump(stream, n+2);
}


Expression let_class::copy_Expression()
{
	return new let_class(copy_Symbol(identifier), copy_Symbol(type_decl), init->copy_Expression(), body->copy_Expression());
}


void let_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "let\n";
	dump_Symbol(stream, n+2, identifier);
	dump_Symbol(stream, n+2, type_decl);
	init->dump(stream, n+2);
	body->dump(stream, n+2);
}


Expression plus_class::copy_Expression()
{
	return new plus_class(e1->copy_Expression(), e2->copy_Expression());
}


void plus_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "plus\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression sub_class::copy_Expression()
{
	return new sub_class(e1->copy_Expression(), e2->copy_Expression());
}


void sub_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "sub\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression mul_class::copy_Expression()
{
	return new mul_class(e1->copy_Expression(), e2->copy_Expression());
}


void mul_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "mul\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression divide_class::copy_Expression()
{
	return new divide_class(e1->copy_Expression(), e2->copy_Expression());
}


void divide_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "divide\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression neg_class::copy_Expression()
{
	return new neg_class(e1->copy_Expression());
}


void neg_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "neg\n";
	e1->dump(stream, n+2);
}


Expression lt_class::copy_Expression()
{
	return new lt_class(e1->copy_Expression(), e2->copy_Expression());
}


void lt_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "lt\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression eq_class::copy_Expression()
{
	return new eq_class(e1->copy_Expression(), e2->copy_Expression());
}


void eq_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "eq\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression leq_class::copy_Expression()
{
	return new leq_class(e1->copy_Expression(), e2->copy_Expression());
}


void leq_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "leq\n";
	e1->dump(stream, n+2);
	e2->dump(stream, n+2);
}


Expression comp_class::copy_Expression()
{
	return new comp_class(e1->copy_Expression());
}


void comp_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "comp\n";
	e1->dump(stream, n+2);
}


Expression int_const_class::copy_Expression()
{
	return new int_const_class(copy_Symbol(token));
}


void int_const_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "int_const\n";
	dump_Symbol(stream, n+2, token);
}


Expression bool_const_class::copy_Expression()
{
	return new bool_const_class(val);
}


void bool_const_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "bool_const\n";
	dump_Boolean(stream, n+2, val);
}


Expression string_const_class::copy_Expression()
{
	return new string_const_class(copy_Symbol(token));
}


void string_const_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "string_const\n";
	dump_Symbol(stream, n+2, token);
}


Expression new__class::copy_Expression()
{
	return new new__class(copy_Symbol(type_name));
}


void new__class::dump(ostream& stream, int n)
{
	stream << pad(n) << "new_\n";
	dump_Symbol(stream, n+2, type_name);
}


Expression isvoid_class::copy_Expression()
{
	return new isvoid_class(e1->copy_Expression());
}


void isvoid_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "isvoid\n";
	e1->dump(stream, n+2);
}


Expression no_expr_class::copy_Expression()
{
	return new no_expr_class();
}


void no_expr_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "no_expr\n";
}


Expression object_class::copy_Expression()
{
	return new object_class(copy_Symbol(name));
}


void object_class::dump(ostream& stream, int n)
{
	stream << pad(n) << "object\n";
	dump_Symbol(stream, n+2, name);
}

