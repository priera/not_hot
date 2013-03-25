#ifndef COOL_TREE_H
#define COOL_TREE_H
//////////////////////////////////////////////////////////
//
// file: cool-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////

#include <string>

#include "cool.h"
#include "tree.h"
#include "stringtab.h"
#include "typing_errors.h"
#include "main_tables.h"

typedef bool ok;

// define the class for phylum
// define simple phylum - Program
typedef class Program_class *Program;

class Program_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Program(); }
   virtual Program copy_Program() = 0;

	virtual void dump_with_types(ostream&, int) = 0; 
};


// define simple phylum - Class_
typedef class Class__class *Class_;

class ConstraintClassError {
public:
	Class_ c_;
	std::string message_;
	
	ConstraintClassError(Class_ c, std::string message);
};

class AttributesIterator;
class MethodsIterator;

class Class__class : public tree_node {
protected:

	Classes children;
	
	virtual Features get_features() = 0;	
public:
	Class__class(){
		state = NOT_MARKED;
	}

	enum mark_state {NOT_MARKED, IN_PROCESS, PROCESSED};
	mark_state state;
 
   tree_node *copy()		 { return copy_Class_(); }
   virtual Class_ copy_Class_() = 0;

	static bool isValidAncestor(Class_ parent);
	
	virtual Classes get_children() = 0;
	virtual void append_child(Class_ child) = 0;

	virtual Symbol get_name() = 0;
	virtual Symbol get_parent() = 0;
	
	ok markSubclasses();
	virtual void checkTypes(ObjectEnvironment oe) = 0;
	virtual bool overridesBaseClass() = 0;
	
	virtual AttributesIterator getAttributesIterator() = 0;
	virtual MethodsIterator getMethodsIterator() = 0;
	
	virtual Symbol get_filename() = 0;      
	virtual void dump_with_types(ostream&,int) = 0; 

	virtual void checkSemantics(Class_ parent, ConstraintClassError * &error) = 0;
	
};

// define simple phylum - Feature
typedef class Feature_class *Feature;

class Feature_class : public tree_node {
protected:
	Class_ container_class;
public:
   tree_node *copy()		 { return copy_Feature(); }
   virtual Feature copy_Feature() = 0;

	enum Feature_type { METHOD, MEMBER };
	typedef Feature_type types;
	
	Feature_type type;
	void set_container_class(Class_ c) { container_class = c;}
	
	virtual Symbol get_name() = 0;
	
	virtual void checkType(ObjectEnvironment oe) = 0;
	
	virtual void dump_with_types(ostream&,int) = 0; 
};


// define simple phylum - Formal
typedef class Formal_class *Formal;

class Formal_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Formal(); }
   virtual Formal copy_Formal() = 0;

	virtual Symbol get_name() = 0;
	virtual Symbol get_type_decl() = 0;
	
	virtual void dump_with_types(ostream&,int) = 0;
};


// define simple phylum - Expression
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
protected:
	Symbol type;
	
	//These definitions are here in order to avoid an excessive number of constants' table access
	static Symbol const Int; 
	static Symbol const Object;
	static Symbol const Str;
	static Symbol const Bool;
	static Symbol const No_type;
	static Symbol const self;
	static Symbol const SELF_TYPE;
	
public:
	
   tree_node *copy() { return copy_Expression(); }
   virtual Expression copy_Expression() = 0;

	enum EXPR_TYPE { GLOBAL, ASSIGN, STATIC_DISPATCH, DISPATCH, COND, LOOP, TYPCASE, BLOCK, LET, PLUS, SUB, MUL, DIVIDE, NEG, LT, EQ, LEQ, COMP, INT_CONST, BOOL_CONST, STRING_CONST, NEW, ISVOID, NO_EXPR, OBJ};
	EXPR_TYPE ex_type;

	virtual Symbol typeIt(Class_ container_class, ObjectEnvironment oe) = 0;

	Symbol get_type() { return type; };
	EXPR_TYPE get_expr_type() { return ex_type; };
	
	virtual void dump_with_types(ostream&,int) = 0;
	void dump_type(ostream&, int);    
	
	Expression_class() { type = (Symbol) NULL; ex_type = Expression_class::GLOBAL; } 
};


// define simple phylum - Case
typedef class Case_class *Case;

class Case_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Case(); }
   virtual Case copy_Case() = 0;

	virtual void dump_with_types(ostream& ,int) = 0;
};


// define the class for phylum - LIST
// define list phlyum - Classes
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;


// define list phlyum - Features
typedef list_node<Feature> Features_class;
typedef Features_class *Features;


// define list phlyum - Formals
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;


// define list phlyum - Expressions
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;


// define list phlyum - Cases
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


// define the class for constructors
// define constructor - program
class program_class : public Program_class {
protected:
   Classes classes;
public:
   program_class(Classes a1) {
      classes = a1;
   }
   Program copy_Program();
   void dump(ostream& stream, int n);

	void dump_with_types(ostream&, int);   
};


Classes nil_Classes();
Classes append_Classes(Classes, Classes);
Classes single_Classes(Class_);

class method_class;
class attr_class;

template <class TYPE>
class BasicFeatureIterator {
private:
	Features list;
	int i;
	Feature_class::types my_iterator_kind;
	
protected:
	BasicFeatureIterator(Features l, Feature_class::types iterator_kind) : list(l), i(-1), my_iterator_kind(iterator_kind) { 
		i = list->first();
	}

	Feature CurrentFeature(){
		return list->nth(i);
	}
	
public:

	void reset(){
		i = list->first();
	}
	
	bool more() { 
		bool ret = list->more(i);
		if (ret && CurrentFeature()->type != my_iterator_kind){
			move();
			ret = more();
		} 
		return ret;
	}
	
	void move() {
		i = list->next(i);
	}
	
	virtual TYPE * current() = 0;
	
};

class AttributesIterator : public BasicFeatureIterator <attr_class> {
public:
	AttributesIterator(Features f);
	
	attr_class * current();
};

class MethodsIterator : public BasicFeatureIterator <method_class> {
public:
	MethodsIterator(Features f);
	
	method_class * current();
};

// define constructor - class_
class class__class : public Class__class {
private: 
	bool excemptOfTypecheck();
	void collectAttributes(ObjectEnvironment & oe);
	
	ConstraintClassError * overridingBaseClass();
	ConstraintClassError * checkClassAttributes();
	ConstraintClassError * collectClassSignatures();
	ConstraintClassError * inheritingBaseClass();
	
protected:
   Symbol name;
   Symbol parent;
   Features features;
   Symbol filename;
	Classes children;
	
	bool is_basic_class_;
public:
   
	class__class(Symbol a1, Symbol a2, Features a3, Symbol a4) : class__class(a1, a2, a3, a4, false) { }
   class__class(Symbol a1, Symbol a2, Features a3, Symbol a4, bool is_basic_class) {
		name = a1;
		parent = a2;
		features = a3;
		filename = a4;
		children = nil_Classes();
		is_basic_class_ = is_basic_class;
		GlobalTables::getInstance().stringtable.add_string(name->get_string());
		
	}
   
   Class_ copy_Class_();
   void dump(ostream& stream, int n);

	void append_child(Class_ child);
	Classes get_children();

	Symbol get_name();
	Symbol get_parent();
	
	Features get_features() { return features; };
	
	AttributesIterator getAttributesIterator();
	MethodsIterator getMethodsIterator();

	void checkTypes(ObjectEnvironment oe);
	bool overridesBaseClass();
	
	void checkSemantics(Class_ parent, ConstraintClassError * &error);
	
	Symbol get_filename();
	bool is_basic_class();
	void dump_with_types(ostream&,int); 
};


// define constructor - method
class method_class : public Feature_class {
protected:
   Symbol name;
   Formals formals;
   Symbol return_type;
   Expression expr;
public:
   method_class(Symbol a1, Formals a2, Symbol a3, Expression a4) {
      name = a1;
      formals = a2;
      return_type = a3;
      expr = a4;
		type = Feature_class::METHOD;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

	Symbol get_name() { return name; }
	Formals get_formals() { return formals; }
	Symbol get_return_type() {return return_type; }	
	Expression get_expr() {return expr; }
	
	//check that there's not a pair or formals in method's definition with the same name
	bool hasValidDefinition();
	bool wellDefinedOverriding(MethodSignature previos_declaration);
	
	void checkType(ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int); 

};


// define constructor - attr
class attr_class : public Feature_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression init;
public:
   attr_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      init = a3;
		type = Feature_class::MEMBER;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

	Symbol get_name() {return name; }
	Symbol get_type_decl() {return type_decl;}
	Expression get_init_expr() { return init;}
	bool has_init_expr() {return init->get_expr_type() != Expression_class::NO_EXPR; }
	void checkType(ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int); 

};


// define constructor - formal
class formal_class : public Formal_class {
protected:
   Symbol name;
   Symbol type_decl;
public:
   formal_class(Symbol a1, Symbol a2) {
      name = a1;
      type_decl = a2;
   }
   Formal copy_Formal();
   void dump(ostream& stream, int n);

	Symbol get_name() {return name; }
	Symbol get_type_decl() {return type_decl; }

	void dump_with_types(ostream&,int);
	
};

// define constructor - branch
class branch_class : public Case_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression expr;
public:
   branch_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      expr = a3;
   }
   Case copy_Case();
   void dump(ostream& stream, int n);

	Symbol get_name() {return name; };
	Symbol get_type_decl(){return type_decl; };
	Expression get_expr(){return expr; };
	
	void dump_with_types(ostream& ,int);

};


// define constructor - assign
class assign_class : public Expression_class {
protected:
   Symbol name;
   Expression expr;
public:
   assign_class(Symbol a1, Expression a2) {
      name = a1;
      expr = a2;
		ex_type = Expression_class::ASSIGN;	
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Expression get_expr() { return expr; };
	Symbol get_name() { return name; };
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - static_dispatch
class static_dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol type_name;
   Symbol name;
   Expressions actual;
public:
   static_dispatch_class(Expression a1, Symbol a2, Symbol a3, Expressions a4) {
      expr = a1;
      type_name = a2;
      name = a3;
      actual = a4;
		ex_type = Expression_class::STATIC_DISPATCH;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Expression get_expr() { return expr; };
	Expressions get_actuals() { return actual; };
	Symbol get_name() { return name; };
	Symbol get_type_name() { return type_name; };
	void dump_with_types(ostream&,int);

};


// define constructor - dispatch
class dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol name;
   Expressions actual;
public:
   dispatch_class(Expression a1, Symbol a2, Expressions a3) {
      expr = a1;
      name = a2;
      actual = a3;
		ex_type = Expression_class::DISPATCH;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Expression get_expr() { return expr; };
	Expressions get_actuals() { return actual; };
	Symbol get_name() { return name; };
	
	void dump_with_types(ostream&,int);

};


// define constructor - cond
class cond_class : public Expression_class {
protected:
   Expression pred;
   Expression then_exp;
   Expression else_exp;
public:
   cond_class(Expression a1, Expression a2, Expression a3) {
      pred = a1;
      then_exp = a2;
      else_exp = a3;
		ex_type = Expression_class::COND;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Expression get_pred() { return pred; };
	Expression get_then() { return then_exp; };
	Expression get_else() { return else_exp; };
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);

};


// define constructor - loop
class loop_class : public Expression_class {
protected:
   Expression pred;
   Expression body;
public:
   loop_class(Expression a1, Expression a2) {
      pred = a1;
      body = a2;
		ex_type = Expression_class::LOOP;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Expression get_pred() { return pred; };
	Expression get_body() { return body; };
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);

};


// define constructor - typcase
class typcase_class : public Expression_class {
protected:
   Expression expr;
   Cases cases;
public:
   typcase_class(Expression a1, Cases a2) {
      expr = a1;
      cases = a2;
		ex_type = Expression_class::TYPCASE;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Expression get_expr() { return expr; };
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - block
class block_class : public Expression_class {
protected:
   Expressions body;
public:
   block_class(Expressions a1) {
      body = a1;
		ex_type = Expression_class::BLOCK;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Expressions get_body() { return body; };
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - let
class let_class : public Expression_class {
protected:
   Symbol identifier;
   Symbol type_decl;
   Expression init;
   Expression body;
public:
   let_class(Symbol a1, Symbol a2, Expression a3, Expression a4) {
      identifier = a1;
      type_decl = a2;
      init = a3;
      body = a4;
		ex_type = Expression_class::LET;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Symbol get_identifier() { return identifier; };
	Expression get_init() { return init; };
	Expression get_body() { return body; };
	Symbol get_type_decl() { return type_decl; };
	
	void dump_with_types(ostream&,int);
   
};

class BinaryArithmeticExpression : public Expression_class {
protected:
	Expression e1;
	Expression e2;
public:
	BinaryArithmeticExpression(Expression a1, Expression a2, EXPR_TYPE type){
		e1 = a1;
		e2 = a2;
		ex_type = type;
	}
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Expression get_ex1() { return e1; }
	Expression get_ex2() { return e2; }
	
};

class BinaryRelationalExpression : public Expression_class {
protected:
	Expression e1;
	Expression e2;
public:
	BinaryRelationalExpression(Expression a1, Expression a2, EXPR_TYPE type){
		e1 = a1;
		e2 = a2;
		ex_type = type;
	}
	
	Expression get_ex1() { return e1; }
	Expression get_ex2() { return e2; }
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
};

// define constructor - plus
class plus_class : public BinaryArithmeticExpression {
public:
	plus_class(Expression a1, Expression a2) : BinaryArithmeticExpression(a1, a2, Expression_class::PLUS) {  }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - sub
class sub_class : public BinaryArithmeticExpression {
public:
	sub_class(Expression a1, Expression a2) : BinaryArithmeticExpression(a1, a2, Expression_class::SUB) { }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	void dump_with_types(ostream&,int);
	
};


// define constructor - mul
class mul_class : public BinaryArithmeticExpression {
public:
	mul_class(Expression a1, Expression a2) : BinaryArithmeticExpression(a1, a2, Expression_class::MUL) { }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - divide
class divide_class : public BinaryArithmeticExpression {
public:
	divide_class(Expression a1, Expression a2) : BinaryArithmeticExpression(a1, a2, Expression_class::DIVIDE) { }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	void dump_with_types(ostream&,int);
   
};

// define constructor - lt
class lt_class : public BinaryRelationalExpression {
public:
	lt_class(Expression a1, Expression a2) : BinaryRelationalExpression(a1, a2, Expression_class::LT) { }
	Expression copy_Expression();
	void dump(ostream& stream, int n);
	
	void dump_with_types(ostream&,int);
	
};

// define constructor - leq
class leq_class : public BinaryRelationalExpression {
public:
	leq_class(Expression a1, Expression a2) : BinaryRelationalExpression(a1, a2, Expression_class::LEQ) { }
	Expression copy_Expression();
	void dump(ostream& stream, int n);
	
	void dump_with_types(ostream&,int);
	
};

// define constructor - neg
class neg_class : public Expression_class {
protected:
   Expression e1;
public:
   neg_class(Expression a1) {
      e1 = a1;
		ex_type = Expression_class::NEG;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Expression get_ex() { return e1; }
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - eq
class eq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   eq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
		ex_type = Expression_class::EQ;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Expression get_ex1() { return e1; }
	Expression get_ex2() { return e2; }
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - comp
class comp_class : public Expression_class {
protected:
   Expression e1;
public:
   comp_class(Expression a1) {
      e1 = a1;
		ex_type = Expression_class::COMP;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
	
	Expression get_ex() { return e1; }
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - int_const
class int_const_class : public Expression_class {
protected:
   Symbol token;
public:
   int_const_class(Symbol a1) {
      token = a1;
		ex_type = Expression_class::INT_CONST;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Symbol get_token() {return token; }
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - bool_const
class bool_const_class : public Expression_class {
protected:
   Boolean val;
public:
   bool_const_class(Boolean a1) {
      val = a1;
		ex_type = Expression_class::BOOL_CONST;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	bool get_val() { return val == 1; };
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - string_const
class string_const_class : public Expression_class {
protected:
   Symbol token;
public:
   string_const_class(Symbol a1) {
      token = a1;
		ex_type = Expression_class::STRING_CONST;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Symbol get_token() {return token; };
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - new_
class new__class : public Expression_class {
protected:
   Symbol type_name;
public:
   new__class(Symbol a1) {
      type_name = a1;
		ex_type = Expression_class::NEW;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Symbol get_type_name() { return type_name; };
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - isvoid
class isvoid_class : public Expression_class {
protected:
   Expression e1;
public:
   isvoid_class(Expression a1) {
      e1 = a1;
		ex_type = Expression_class::ISVOID;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	Expression get_ex() {return e1; }
	
	void dump_with_types(ostream&,int);
   
};


// define constructor - no_expr
class no_expr_class : public Expression_class {
protected:
public:
   no_expr_class() {
		ex_type = Expression_class::NO_EXPR;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);

	void dump_with_types(ostream&,int);
   
};


// define constructor - object
class object_class : public Expression_class {
protected:
	Symbol name;
	
public:
	
	object_class(Symbol a1) {
		name = a1;		
		ex_type = Expression_class::OBJ;
		
	}
	Expression copy_Expression();
	void dump(ostream& stream, int n);
	
	Symbol get_name() {return name; }
	
	Symbol typeIt(Class_ container_class, ObjectEnvironment oe);
	
	void dump_with_types(ostream&,int);
};

// define the prototypes of the interface
Features nil_Features();
Features single_Features(Feature);
Features append_Features(Features, Features);
Formals nil_Formals();
Formals single_Formals(Formal);
Formals append_Formals(Formals, Formals);
Expressions nil_Expressions();
Expressions single_Expressions(Expression);
Expressions append_Expressions(Expressions, Expressions);
Cases nil_Cases();
Cases single_Cases(Case);
Cases append_Cases(Cases, Cases);
Program program(Classes);
Class_ class_(Symbol, Symbol, Features, Symbol);
Class_ basic_class_(Symbol, Symbol, Features, Symbol);
Feature method(Symbol, Formals, Symbol, Expression);
Feature attr(Symbol, Symbol, Expression);
Formal formal(Symbol, Symbol);
Case branch(Symbol, Symbol, Expression);
Expression assign(Symbol, Expression);
Expression static_dispatch(Expression, Symbol, Symbol, Expressions);
Expression dispatch(Expression, Symbol, Expressions);
Expression cond(Expression, Expression, Expression);
Expression loop(Expression, Expression);
Expression typcase(Expression, Cases);
Expression block(Expressions);
Expression let(Symbol, Symbol, Expression, Expression);
Expression plus(Expression, Expression);
Expression sub(Expression, Expression);
Expression mul(Expression, Expression);
Expression divide(Expression, Expression);
Expression neg(Expression);
Expression lt(Expression, Expression);
Expression eq(Expression, Expression);
Expression leq(Expression, Expression);
Expression comp(Expression);
Expression int_const(Symbol);
Expression bool_const(Boolean);
Expression string_const(Symbol);
Expression new_(Symbol);
Expression isvoid(Expression);
Expression no_expr();
Expression object(Symbol);


#endif
