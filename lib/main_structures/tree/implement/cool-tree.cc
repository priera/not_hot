#include "cool-tree.h"

AttributesIterator::AttributesIterator(Features f) : BasicFeatureIterator(f, Feature_class::MEMBER) { }
	
attr_class * AttributesIterator::current(){
	return (attr_class *)CurrentFeature();
}

MethodsIterator::MethodsIterator(Features f) : BasicFeatureIterator(f, Feature_class::METHOD) { }

method_class * MethodsIterator::current() {
	return (method_class *)CurrentFeature();
}

bool Class__class::isValidAncestor(Class_ parent){
	Constants constants = GlobalTables::getInstance().get_constants();
	Symbol s = parent->get_name();
	return (s != constants.Bool && s!= constants.Int && s != constants.Str);
}

ok Class__class::markSubclasses() {
	ok ok = true;
	state = Class__class::IN_PROCESS;
	
	Class_ temp = NULL;
	Classes children = get_children();
	for (int i = children->first(); children->more(i); i = children->next(i)){
		
		temp = children->nth(i);
		if (temp->state == Class__class::IN_PROCESS){
			ok = false;
			break;
		}
		
		ok = temp->markSubclasses() && ok;
 	}
	
	state = Class__class::PROCESSED;
	
	return ok;
}

Symbol class__class::get_filename() { 
	return filename;
}

AttributesIterator class__class::getAttributesIterator(){
	return AttributesIterator(get_features());
}

MethodsIterator class__class::getMethodsIterator(){
	return MethodsIterator(get_features());
}

void class__class::collectAttributes(ObjectEnvironment & oe){
	AttributesIterator iter = getAttributesIterator();
	attr_class *attr;
	while (iter.more()){
		attr = iter.current();
		
		oe.addid(attr->get_name(), attr->get_type_decl());
		
		iter.move();
	}

}

bool class__class::excemptOfTypecheck(){
	Symbol name = get_name();
	Constants constants = GlobalTables::getInstance().get_constants();
	return (name == constants.Bool || name == constants.Int || name == constants.IO || name == constants.Object  || name == constants.Str);
}

ConstraintClassError::ConstraintClassError(Class_ c, std::string message) : c_(c), message_(message) {};

ConstraintClassError * class__class::checkClassAttributes(){
	
	ConstraintClassError * ret = NULL;
	Symbol self_constant = GlobalTables::getInstance().get_constants().self;
	
	AttributesIterator iter = getAttributesIterator();
	attr_class *attr;
	
	bool error = false;
	
	ObjectEnvironment &oe = ObjectEnvironment::getInstance();
	
	while (iter.more() && !error ){
		attr = iter.current();
		
		if (oe.probe(attr->get_name()) != NULL) {
			ret = new ConstraintClassError(this, "Attribute defined more than once");
			error = true;
		} else if (oe.lookup(attr->get_name()) != NULL ){
			ret = new ConstraintClassError(this, "Overriding parent class attribute");
			error = true;
		}else if (attr->get_name() == self_constant) {
			ret = new ConstraintClassError(this, "'self' cannot be the name of an attribute");
			error = true;
		}else if (!ClassTable::getInstance().validTypeName(attr->get_type_decl())) {
			cout << attr->get_name() <<  " " << attr->get_type_decl() << endl;
			ret = new ConstraintClassError(this, "attribute's type hasn't been declared");
			error = true;
		}else {
			oe.addid(attr->get_name(), attr->get_type_decl());
		}
		iter.move();
	}
	
	return ret;
}

ConstraintClassError * class__class::collectClassSignatures(){
	
	ConstraintClassError * ret = NULL;
	
	MethodsIterator iter = getMethodsIterator();
	ClassMethods & signatures = ClassMethods::getInstance();
	method_class *m;
	MethodSignatureKey_ *sk;
	bool error = false;
	
	while (iter.more() && !error) {
		m = iter.current();
		
		if (!m->hasValidDefinition()){
			ret = new ConstraintClassError(this, "Bad signature");
			error = true;
			break;
		}
		
		if (!ClassTable::getInstance().validTypeName(m->get_return_type())){
			ret = new ConstraintClassError(this, "Method's return type hasn't been declared");
			error = true;
		}
		
		sk = new MethodSignatureKey_(this, m->get_name());
		
		if (signatures.probe(*sk) != NULL) {
			ret = new ConstraintClassError(this, "Method defined more than once");
			error = true;
			break;
		}
		
		MethodSignature overr = signatures.getSignature(*sk);
		if (overr != NULL && !m->wellDefinedOverriding(overr)){
			ret = new ConstraintClassError(this, "Method overriding doesn't conform its base definition");
			error = true;
			break;
		}
		
		MethodSignature ms = new MethodSignature_(m->get_formals(), m->get_return_type());
		signatures.addid(*sk, ms);
		
		iter.move();
	}
	
	return ret;
}

ConstraintClassError * class__class::inheritingBaseClass(){
	Constants c = GlobalTables::getInstance().get_constants();
	ConstraintClassError * ret = NULL;
	if (parent == c.Str || parent == c.Int || parent == c.Bool){
		ret = new ConstraintClassError(this, std::string("Inheriting from class ") + parent->get_string() + " is not allowed");
	}
	return ret;
}

bool class__class::is_basic_class() {
	return is_basic_class_;
}

bool class__class::overridesBaseClass(){
	if (is_basic_class_) return false;
	
	Constants c = GlobalTables::getInstance().get_constants();
	if (name == c.Object || name == c.Int || name == c.Bool || name == c.Str) return true;
		
	return false;
}

void class__class::checkSemantics(Class_ parent, ConstraintClassError * &error){
	ObjectEnvironment &oe = ObjectEnvironment::getInstance();
	oe.enterscope();
	
	if ((error = inheritingBaseClass()) != NULL) return;
	if ((error = checkClassAttributes()) != NULL) return ;	
	if ((error = collectClassSignatures()) != NULL) return ;

	bool error_in_child = false;
	int i = children->first();
	while (!error_in_child && children->more(i)){
		children->nth(i)->checkSemantics(this, error);
		error_in_child = (error != NULL);
		i = children->next(i);
	}

	oe.exitscope();
}

void class__class::checkTypes(ObjectEnvironment oe){
	oe.enterscope();
	
	Symbol SELF_TYPE = GlobalTables::getInstance().get_constants().SELF_TYPE;
	Symbol self = GlobalTables::getInstance().get_constants().self;
	oe.addid(self, SELF_TYPE);
	collectAttributes(oe);

	TypingErrorEnvironment::getCurrentEnvironment().set_classname(std::string(name->get_string()));
	TypingErrorEnvironment::getCurrentEnvironment().set_filename(std::string(filename->get_string()));
	
	if (!excemptOfTypecheck()) {
		Features features = get_features();

		for (int i = features->first(); features->more(i); i = features->next(i)){
			features->nth(i)->checkType(oe);
		}
	}
	
	Classes children = get_children();
	for (int i = children->first(); children->more(i); i = children->next(i)){
		children->nth(i)->checkTypes(oe);
	}
	
	oe.exitscope();
}

void class__class::append_child(Class_ child){
	children = append_Classes(this->children, single_Classes(child));
};

Classes class__class::get_children() {
	return this->children;
};

Symbol class__class::get_parent() {
	return this->parent;
};

Symbol class__class::get_name(){
	return this->name;
}

bool method_class::hasValidDefinition(){
	Formals f = get_formals();
	Symbol current_name;
	Symbol current_type;
	int i = f->first();
	int j = 0;
	bool ok = true;
	while (ok && f->more(i)){
		current_name = f->nth(i)->get_name();
		current_type = f->nth(i)->get_type_decl();
		if (current_name == GlobalTables::getInstance().get_constants().self) ok = false;
		if (current_type == GlobalTables::getInstance().get_constants().SELF_TYPE) ok = false;
		
		j = f->first();
		while (ok && f->more(j)){
			if (current_name == f->nth(j)->get_name() && i!=j) ok = false;
			j= f->next(j);
		}
		i = f->next(i);
	}
	
	return ok;
}

bool method_class::wellDefinedOverriding(MethodSignature previos_declaration){
	if (return_type != previos_declaration->returnType)
		return false;
	
	bool types_are_same = true;
	
	Formals previos_formals = previos_declaration->formals;
	int previos_formals_count = previos_formals->first();
	int my_formals_count = formals->first();
	
	while (types_are_same && formals->more(my_formals_count) && previos_formals->more(previos_formals_count)){
		if (formals->nth(my_formals_count)->get_type_decl() != previos_formals->nth(previos_formals_count)->get_type_decl()){
			types_are_same = false;
		}
		
		previos_formals_count = previos_formals->next(previos_formals_count);
		my_formals_count = formals->next(my_formals_count);
	}
	
	if (!types_are_same) return false;
	
	bool my_formals_exhausted = (formals->more(my_formals_count) == false );
	bool previos_formals_exhausted = (previos_formals->more(previos_formals_count) == false);
	
	if (!(my_formals_exhausted && previos_formals_exhausted)) return false;
	
	return true;
}

void method_class::checkType(ObjectEnvironment oe){
	TypingErrorEnvironment::getCurrentEnvironment().set_scope(TypingErrorEnvironment::method);
	
	SignatureKey sk = new MethodSignatureKey_(container_class, get_name());
	
	MethodSignature ms = ClassMethods::getInstance().lookup(*sk);
	
	oe.enterscope();
	Formals formals = ms->formals;
	
	bool error = false;
	
	for (int i = formals->first(); formals->more(i); i = formals->next(i)){
		Formal_class *f;
		
		f = formals->nth(i);
		oe.addid(f->get_name(), f->get_type_decl());
	}
	
	Symbol SELF_TYPE = GlobalTables::getInstance().get_constants().SELF_TYPE;
	Symbol declared_type = ms->returnType;
	Symbol expression_type;
	
	if (declared_type == SELF_TYPE) {
		declared_type = container_class->get_name();
	}
	
	expression_type = get_expr()->typeIt(container_class, oe);

	if (expression_type == SELF_TYPE) {
		expression_type = container_class->get_name();
	}

	if (!ClassTable::getInstance().isSubtypeOf(expression_type, declared_type)){
			error = true;
			registerTypingError(std::string("Method's return type doesn't conform its declared type"));
	}
	
	oe.exitscope();
	
}

void attr_class::checkType(ObjectEnvironment oe){
	TypingErrorEnvironment::getCurrentEnvironment().set_scope(TypingErrorEnvironment::attribute);
	
	Symbol declared_type = get_type_decl();
	
	//These following two assignments are here in order to make the code more readable
	Symbol SELF_TYPE = GlobalTables::getInstance().get_constants().SELF_TYPE;
	Symbol No_type = GlobalTables::getInstance().get_constants().No_type;
	
	if (declared_type == SELF_TYPE) {
		declared_type = container_class->get_name();
	}
	oe.enterscope();

	Symbol dynamic_type;
	dynamic_type = get_init_expr()->typeIt(container_class, oe);
	
	if (dynamic_type == No_type || dynamic_type == SELF_TYPE){
		dynamic_type = declared_type;
	} 
	
	if (!ClassTable::getInstance().isSubtypeOf(dynamic_type, declared_type)) {
		cout <<"name " << name << " dynamic_type: " << dynamic_type << " declared: " << declared_type << endl;
		registerTypingError(std::string("Attribute's return type doesn't conform its declared type"));
	}
	
	oe.exitscope();
}

Symbol const Expression_class::Int = GlobalTables::getInstance().get_constants().Int;
Symbol const Expression_class::Object = GlobalTables::getInstance().get_constants().Object;
Symbol const Expression_class::Str = GlobalTables::getInstance().get_constants().Str;
Symbol const Expression_class::Bool = GlobalTables::getInstance().get_constants().Bool;
Symbol const Expression_class::No_type = GlobalTables::getInstance().get_constants().No_type;
Symbol const Expression_class::self = GlobalTables::getInstance().get_constants().self;
Symbol const Expression_class::SELF_TYPE = GlobalTables::getInstance().get_constants().SELF_TYPE;

Symbol assign_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	bool error = false;

	if (oe.lookup(name) == NULL){
		error = true;
		registerTypingError(std::string("Assignment's identifier ") + std::string(name->get_string()) + std::string(" has not been declared"));
	} else if (name == GlobalTables::getInstance().get_constants().self){
		error = true;
		registerTypingError(std::string("Cannot assign to 'self'"));
	}
	
	Symbol t1 = expr->typeIt(container_class, oe);
	
	if (!error) {
		
		Symbol temp = oe.lookup(name);
		
		if (!(ClassTable::getInstance().isSubtypeOf(t1, temp))){
			error = true;
			
			registerTypingError(std::string("Inferred type of assignment expression doesn't conform its declared type"));
		}
	}
	
	if (error) {
		t1 = Object;
	}
	
	type = t1;
	return type;
}

Symbol static_dispatch_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	bool error = false;
	
	Symbol expr_dynamic_type = expr->typeIt(container_class, oe);
	
	if (expr_dynamic_type == SELF_TYPE) expr_dynamic_type = container_class->get_name();
	
	if (!(ClassTable::getInstance().isSubtypeOf(expr_dynamic_type, type_name))){
		error = true;
		registerTypingError(std::string("Inferred type of expression doesn't conform method type"));
		type = Object;
		return type;
	}
	
	SignatureKey sk = new MethodSignatureKey_(ClassTable::getInstance().lookup(expr_dynamic_type), name);
	MethodSignature ms = ClassMethods::getInstance().getSignature(*sk);
	
	Formals formals = ms->formals;
	
	formal_class *f;
	Symbol temp;
	
	for (int i = formals->first(); formals->more(i); i = formals->next(i)){
		f = (formal_class*)formals->nth(i);
		temp = actual->nth(i)->typeIt(container_class, oe);
		if (temp == SELF_TYPE) temp = container_class->get_name();
		if (!(ClassTable::getInstance().isSubtypeOf(temp, f->get_type_decl()))) error = true;
	}
	
	Symbol dynamic_type = ms->returnType;
	if (dynamic_type == SELF_TYPE){
		dynamic_type = expr_dynamic_type;
	}
	
	if (error) {
		registerTypingError(std::string("Infered type of parameter in dispatch doesn't conform type of method declaration"));
		dynamic_type = Object;
	}
	
	type = dynamic_type;
	return type;
}

Symbol dispatch_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	
	Symbol expr_dynamic_type = expr->typeIt(container_class, oe);
	Symbol class_signature_type = expr_dynamic_type;
	
	if (class_signature_type == SELF_TYPE){
		class_signature_type = container_class->get_name();
	}
	
	SignatureKey sk = new MethodSignatureKey_(ClassTable::getInstance().lookup(class_signature_type), name);
	MethodSignature ms = ClassMethods::getInstance().getSignature(*sk);
	
	if (ms == NULL) {
		std::string error_message = std::string("Signature of method \"") + std::string(name->get_string()) + std::string("\" within class \"") \
		+ std::string(class_signature_type->get_string()) + std::string("\" doesn't exist");
		registerTypingError(error_message);
		type = Object;
		return type;
	}
	
	Formals formals = ms->formals;
	formal_class *f;
	Symbol temp;
	bool error = false;
	
	for (int i = formals->first(); formals->more(i); i = formals->next(i)){
		f = (formal_class*)formals->nth(i);
		temp = actual->nth(i)->typeIt(container_class, oe);
		if (temp == SELF_TYPE) temp = container_class->get_name();
		if (!(ClassTable::getInstance().isSubtypeOf(temp, f->get_type_decl()))) error = true;
	}
	
	Symbol dynamic_type = ms->returnType;
	if (dynamic_type == SELF_TYPE){
		dynamic_type = expr_dynamic_type;
	} 
	
	if (error) {
		registerTypingError(std::string("Infered type of parameter in dispatch doesn't conform type of method declaration"));
		dynamic_type = Object;
	}
	
	type = dynamic_type;
	return type;	
}

Symbol cond_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	bool error = (pred->typeIt(container_class, oe) != Bool);
	
	if (error) {
		registerTypingError(std::string("Conditional predicate must be of type Bool"));
	}
	
	Symbol t1 = then_exp->typeIt(container_class, oe);
	Symbol t2 = else_exp->typeIt(container_class, oe);
	
	Symbol ret = ((error) ? Object : ClassTable::getInstance().lub(t1, t2));
	type = ret;
	return type;
}

Symbol loop_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	if (pred->typeIt(container_class, oe) != Bool) {
		registerTypingError(std::string("Loop predicate must be of type Bool"));
	}
	
	body->typeIt(container_class, oe);
	
	type = Object;
	return type;
}

Symbol typcase_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	
	expr->typeIt(container_class, oe);
	
	bool error = false;
	branch_class *cur_branch;
	Symbol aux_type = NULL;
	Symbol cur_infered_type;
	Symbol cur_declared_type;
	int j;
	
	for (int i = cases->first(); cases->more(i); i=cases->next(i)){
		cur_branch = (branch_class *)cases->nth(i);
		
		oe.enterscope();
		oe.addid(cur_branch->get_name(), cur_branch->get_type_decl());
		cur_infered_type = cur_branch->get_expr()->typeIt(container_class, oe);
		cur_declared_type = cur_branch->get_type_decl();
		
		if (aux_type != NULL){
			aux_type = ClassTable::getInstance().lub(aux_type, cur_infered_type);
		} else {
			aux_type = cur_infered_type;
		}
		oe.exitscope();
		
		j =  cases->first();
		bool type_repeated = false;
		while (j < i && !type_repeated){
			if (((branch_class *)cases->nth(j))->get_type_decl() == cur_declared_type) type_repeated = true;
			j = cases->next(j);
		}
		cout << endl;
		if (type_repeated) {
			error = true;
			registerTypingError(std::string("Each \"case\" branch must have a different type"));
			aux_type = Object;
		}
		
	}
	
	type = aux_type;
	return type;
}

Symbol block_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	Symbol s;
	for (int i = body->first(); body->more(i); i = body->next(i)){
		s = body->nth(i)->typeIt(container_class, oe);
	}
	
	type = s;
	return type;
}

Symbol let_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	Symbol infered_type = type_decl;
	
	if (infered_type == SELF_TYPE) {
		infered_type = container_class->get_name();
	}
	
	bool error = false;
	std::string error_message;
	
	Symbol infered_init_type = init->typeIt(container_class, oe); 
	if (infered_init_type == SELF_TYPE){
		infered_init_type = container_class->get_name();
	}
	
	if (infered_init_type != No_type && !ClassTable::getInstance().isSubtypeOf(infered_init_type, infered_type)){
		error = true;	
		error_message = std::string("Infered type of let expression is not a subtype of its declared type");
	}
	
	if (identifier == self){
		error = true;
		error_message = std::string("Declared variable in a \"let\" expression cannot be named as \"self\"");
	}
	
	Symbol s = NULL;
	if (error) {
		registerTypingError(error_message);
		s = Object;
	} else {
		oe.enterscope();
		oe.addid(identifier, type_decl);
		s = body->typeIt(container_class, oe);
		oe.exitscope();
		
	}
	
	type = s;
	return type;
	
}

Symbol BinaryArithmeticExpression::typeIt(Class_ container_class, ObjectEnvironment oe){
	Symbol s1 = e1->typeIt(container_class, oe);
	Symbol s2 = e2->typeIt(container_class, oe);
	
	Symbol s = Int;
	if (s1 != Int || s2 != Int) {
		registerTypingError(std::string("In an arithmetic expression, both operands must be of type Int"));
		s = Object;
	}
	
	type = s;
	return type;
}

Symbol BinaryRelationalExpression::typeIt(Class_ container_class, ObjectEnvironment oe) {
	Symbol s1 = e1->typeIt(container_class, oe);
	Symbol s2 = e2->typeIt(container_class, oe);
	
	Symbol s = Bool;
	if (!(s1 == Int && s2 == Int)){
		registerTypingError(std::string("In a arithmetic expression, both operands must be of type Int"));
		s = Object;
	}
	
	type = s;
	return type;
}

Symbol neg_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	Symbol s1 = e1->typeIt(container_class, oe);
	
	Symbol s = Int;
	if (s1 != Int){
		registerTypingError(std::string("In a negation expression, operand must be of type Int"));
		s = Object;
	}
	
	type = s;
	return type;
}

Symbol eq_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	Symbol s1 = e1->typeIt(container_class, oe);
	Symbol s2 = e2->typeIt(container_class, oe);
	
	Symbol s = Bool;
	bool error = false;
	
	if ((s1 == Int && s2 != Int) || (s1 != Int && s2 == Int)) {
		error = true;
		s = Object;
	} else if ((s1 == Str && s2 != Str ) || (s1 != Str && s2 == Str)){
		error = true;
		s = Object;
	} else if ((s1 == Bool && s2 != Bool ) || (s1 != Bool && s2 == Bool)){
		error = true;
		s = Object;
	}
	
	if (error)
		registerTypingError(std::string("When types Int, Str or Bool are involved in an equality test, both operands must have same type"));
	
	type = s;
	return type;
}

Symbol comp_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	
	//NOT operation
	Symbol actual_type = e1->typeIt(container_class, oe);
	Symbol s = Bool;
	if (actual_type != s){
		registerTypingError(std::string("operand of NOT doesn't have boolean type"));
		s = Object;
	}
	type = s;
	return s;
}

Symbol int_const_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	type = Int;
	return type;
}

Symbol bool_const_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	type = Bool;
	return type;
}

Symbol string_const_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	type = Str;
	return type;
}

Symbol new__class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	if (!ClassTable::getInstance().validTypeName(type_name)){
		registerTypingError(std::string("Type name hasn't been defined"));
		type = Object;
	} else {
		type = type_name;		
	}
	return type;
}

Symbol isvoid_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	e1->typeIt(container_class, oe);
	type = Bool;
	return type;
}

Symbol no_expr_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	type = GlobalTables::getInstance().get_constants().No_type;
	return type;
}

Symbol object_class::typeIt(Class_ container_class, ObjectEnvironment oe) {
	Symbol s = oe.lookup(name);
	
	if (s == NULL) {
		registerTypingError(std::string("Not declared identifier \"") + std::string(name->get_string()) + std::string("\""));
		//Error recovery strategy: assign object to a non-defined identifier
		s = Object;
	}
	type = s;
	return s;
}

// interfaces used by Bison
Classes nil_Classes()
{
   return new nil_node<Class_>();
}

Classes single_Classes(Class_ e)
{
   return new single_list_node<Class_>(e);
}

Classes append_Classes(Classes p1, Classes p2)
{
   return new append_node<Class_>(p1, p2);
}

Features nil_Features()
{
   return new nil_node<Feature>();
}

Features single_Features(Feature e)
{
   return new single_list_node<Feature>(e);
}

Features append_Features(Features p1, Features p2)
{
   return new append_node<Feature>(p1, p2);
}

Formals nil_Formals()
{
   return new nil_node<Formal>();
}

Formals single_Formals(Formal e)
{
   return new single_list_node<Formal>(e);
}

Formals append_Formals(Formals p1, Formals p2)
{
   return new append_node<Formal>(p1, p2);
}

Expressions nil_Expressions()
{
   return new nil_node<Expression>();
}

Expressions single_Expressions(Expression e)
{
   return new single_list_node<Expression>(e);
}

Expressions append_Expressions(Expressions p1, Expressions p2)
{
   return new append_node<Expression>(p1, p2);
}

Cases nil_Cases()
{
   return new nil_node<Case>();
}

Cases single_Cases(Case e)
{
   return new single_list_node<Case>(e);
}

Cases append_Cases(Cases p1, Cases p2)
{
   return new append_node<Case>(p1, p2);
}

Program program(Classes classes)
{
  return new program_class(classes);
}

Class_ class_(Symbol name, Symbol parent, Features features, Symbol filename)
{
  return new class__class(name, parent, features, filename);
}

Class_ basic_class_(Symbol name, Symbol parent, Features features, Symbol filename){
	return new class__class(name, parent, features, filename, true);
}

Feature method(Symbol name, Formals formals, Symbol return_type, Expression expr)
{
  return new method_class(name, formals, return_type, expr);
}

Feature attr(Symbol name, Symbol type_decl, Expression init)
{
  return new attr_class(name, type_decl, init);
}

Formal formal(Symbol name, Symbol type_decl)
{
  return new formal_class(name, type_decl);
}

Case branch(Symbol name, Symbol type_decl, Expression expr)
{
  return new branch_class(name, type_decl, expr);
}

Expression assign(Symbol name, Expression expr)
{
  return new assign_class(name, expr);
}

Expression static_dispatch(Expression expr, Symbol type_name, Symbol name, Expressions actual)
{
  return new static_dispatch_class(expr, type_name, name, actual);
}

Expression dispatch(Expression expr, Symbol name, Expressions actual)
{
  return new dispatch_class(expr, name, actual);
}

Expression cond(Expression pred, Expression then_exp, Expression else_exp)
{
  return new cond_class(pred, then_exp, else_exp);
}

Expression loop(Expression pred, Expression body)
{
  return new loop_class(pred, body);
}

Expression typcase(Expression expr, Cases cases)
{
  return new typcase_class(expr, cases);
}

Expression block(Expressions body)
{
  return new block_class(body);
}

Expression let(Symbol identifier, Symbol type_decl, Expression init, Expression body)
{
  return new let_class(identifier, type_decl, init, body);
}

Expression plus(Expression e1, Expression e2)
{
  return new plus_class(e1, e2);
}

Expression sub(Expression e1, Expression e2)
{
  return new sub_class(e1, e2);
}

Expression mul(Expression e1, Expression e2)
{
  return new mul_class(e1, e2);
}

Expression divide(Expression e1, Expression e2)
{
  return new divide_class(e1, e2);
}

Expression neg(Expression e1)
{
  return new neg_class(e1);
}

Expression lt(Expression e1, Expression e2)
{
  return new lt_class(e1, e2);
}

Expression eq(Expression e1, Expression e2)
{
  return new eq_class(e1, e2);
}

Expression leq(Expression e1, Expression e2)
{
  return new leq_class(e1, e2);
}

Expression comp(Expression e1)
{
  return new comp_class(e1);
}

Expression int_const(Symbol token)
{
  return new int_const_class(token);
}

Expression bool_const(bool val)
{
  return new bool_const_class(val);
}

Expression string_const(Symbol token)
{
  return new string_const_class(token);
}

Expression new_(Symbol type_name)
{
  return new new__class(type_name);
}

Expression isvoid(Expression e1)
{
  return new isvoid_class(e1);
}

Expression no_expr()
{
  return new no_expr_class();
}

Expression object(Symbol name)
{
  return new object_class(name);
}

