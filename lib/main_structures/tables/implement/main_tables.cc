#include "main_tables.h"

ObjectEnvironment::ObjectEnvironment() : SymbolTable<Symbol, Entry>() {
	; 
}

ObjectEnvironment & ObjectEnvironment::getInstance() {
	static ObjectEnvironment oe;
	return oe;
}

MethodSignature_::MethodSignature_(Formals f_, Symbol rt) : formals(f_), returnType(rt) {
	;
}

MethodSignatureKey_::MethodSignatureKey_ (Class_ c_, Symbol f_){
	c = c_;
	f = f_;
}

ostream & MethodSignatureKey_::print(ostream & s) const {
	return s << "classname :" << (c->get_name()) << endl << "method name: " << f;
}

bool MethodSignatureKey_::operator==(const MethodSignatureKey_ &other) {
	return c->get_name() == other.c->get_name() && f == other.f ;
}

ClassMethods::ClassMethods() : SymbolTable<MethodSignatureKey_, MethodSignature_> (){
	enterscope();
}

ClassMethods & ClassMethods::getInstance(){
	static ClassMethods cm;
	return cm;
}

MethodSignature_ * ClassMethods::getSignature(MethodSignatureKey_ mk) {
	MethodSignature_ *ms = lookup(mk);
	if (ms != NULL) {
		return ms;
	} else if (mk.c->get_parent() == GlobalTables::getInstance().get_constants().No_class) {
		return NULL;
	} else {
		Class_ parent = ClassTable::getInstance().lookup(mk.c->get_parent());
		return getSignature(MethodSignatureKey_(parent, mk.f)); 
	}
}

ClassRegisteringError::ClassRegisteringError(Class_ c, std::string message) : c_(c), message_(message) {}

Symbol const ClassTable::Object = GlobalTables::getInstance().get_constants().Object;

ClassTable::ClassTable() : SymbolTable<Entry *, Class__class> () {
	
	user_classes = nil_Classes();
	main_found = false;
	
	this->enterscope();
	
	predefined_classes = generateBasicClasses();
	all_classes = predefined_classes;
	
}

ostream & ClassTable::print_class_hierarchy(ostream & stream, Class_ c, int tab){
	stream << pad(tab) << c->get_name() << endl;
	for (int i = c->get_children()->first(); c->get_children()->more(i); i = c->get_children()->next(i)){
		print_class_hierarchy(stream, c->get_children()->nth(i), tab+2);
	}
	return stream;
} 

ostream & ClassTable::print(ostream & stream) {

	print_class_hierarchy(stream, base_class, 0);

	stream << endl;
	
	return stream;
}

Classes ClassTable::get_user_classes(){
	return user_classes;
}

Classes ClassTable::get_all_classes(){
	return all_classes;
}

Classes ClassTable::get_predefined_classes(){
	return predefined_classes;
}

Class_ ClassTable::get_base_class(){
	return base_class;
}

bool ClassTable::hasMainClass(){
	return main_found;
}

Symbol ClassTable::climbingLub(Symbol t1, Symbol t2){
	if (t1 == t2) {
		return t1;
	} else if (t1 == Object) {
		return Object;
	} else if (isSubtypeOf(t2, t1)) {
		return t1;
	} else {
		return climbingLub(lookup(t1)->get_parent(), t2);
	}
}

Symbol ClassTable::lub(Symbol t1, Symbol t2){
	if (t1 == t2) {
		return t1;
	} else if (t1 == Object || t2 == Object) {
		return Object;
	} else if (isSubtypeOf(t1, t2)) {
		return t2;
	} else if (isSubtypeOf(t2, t1)) {
		return t1;
	} else {
		return climbingLub(lookup(t1)->get_parent(), t2);
	}
	
}

bool ClassTable::isSubtypeOf(Symbol subtype, Symbol supertype){

	if (subtype == supertype) return true;
	if (supertype == Object) return true;
	
	bool found = false;
	Class_ temp_class;
	Symbol parent_class = subtype;
	
	while (!found && parent_class != Object) {
		temp_class = lookup(parent_class);
		parent_class = temp_class->get_parent();
		if (parent_class == supertype) found = true;
	}
	
	return found;
	
}

bool ClassTable::validTypeName(Symbol s){
	if (s == GlobalTables::getInstance().get_constants().SELF_TYPE) return true;
	if (s == GlobalTables::getInstance().get_constants().prim_slot) return true;
	if (probe(s) == NULL) return false;
	return true;
}

ClassRegisteringError * ClassTable::add_class(Class_ c){
	ClassRegisteringError* ret = NULL;
	
	if (c->overridesBaseClass()){ 
		ret = new ClassRegisteringError(c, std::string("Overriding base class"));
	} else if (c->get_name() == GlobalTables::getInstance().get_constants().SELF_TYPE) { 
		ret = new ClassRegisteringError(c, std::string("SELF_TYPE cannot be used as class name"));
	}else if (probe(c->get_name()) != NULL) {
		ret = new ClassRegisteringError(c, std::string("Class cannot be defined more than once"));
	}
	
	addid(c->get_name(), c);
	
	if (!main_found && c->get_name() == GlobalTables::getInstance().get_constants().Main) {
		main_class = c;
		main_found = true;
	}
	user_classes = append_Classes(user_classes, single_Classes(c));
	all_classes = append_Classes(all_classes, single_Classes(c));
	
	return ret;
}

ClassTable & ClassTable::getInstance() {
	static ClassTable CT;
	return CT;
}

Classes ClassTable::generateBasicClasses() {
	// The tree package uses these globals to annotate the classes built below.
	Symbol filename = GlobalTables::getInstance().stringtable.add_string("<basic_class>");
	Constants constants = GlobalTables::getInstance().get_constants();
	
	// 
	// The Object class has no parent class. Its methods are
	//        abort() : Object    aborts the program
	//        type_name() : Str   returns a string representation of class name
	//        copy() : SELF_TYPE  returns a copy of the object
	//
	// There is no need for method bodies in the basic classes---these
	// are already built in to the runtime system.
	Class_ Object_class =
	basic_class_(constants.Object, 
			 constants.No_class,
		  append_Features(
			  append_Features(
				  single_Features(method(constants.cool_abort, nil_Formals(), constants.Object, no_expr())),
									single_Features(method(constants.type_name, nil_Formals(), constants.Str, no_expr()))),
								single_Features(method(constants.copy, nil_Formals(), constants.SELF_TYPE, no_expr()))),
			 filename);
	base_class = Object_class;
	
	// 
	// The IO class inherits from Object. Its methods are
	//        out_string(Str) : SELF_TYPE       writes a string to the output
	//        out_int(Int) : SELF_TYPE            "    an int    "  "     "
	//        in_string() : Str                 reads a string from the input
	//        in_int() : Int                      "   an int     "  "     "
	//
	Class_ IO_class = 
	basic_class_(constants.IO, 
			 constants.Object,
		  append_Features(
			  append_Features(
				  append_Features(
					  single_Features(method(constants.out_string, single_Formals(formal(constants.arg, constants.Str)),
													 constants.SELF_TYPE, no_expr())),
										single_Features(method(constants.out_int, single_Formals(formal(constants.arg, constants.Int)),
																	  constants.SELF_TYPE, no_expr()))),
									single_Features(method(constants.in_string, nil_Formals(), constants.Str, no_expr()))),
								single_Features(method(constants.in_int, nil_Formals(), constants.Int, no_expr()))),
			 filename);  
	
	//    // The Int class has no methods and only a single attribute, the
	// "val" for the integer. 
	//
	Class_ Int_class =
	basic_class_(constants.Int, 
			 constants.Object,
		  single_Features(attr(constants.val, constants.prim_slot, no_expr())),  filename);
	
	//
	// Bool also has only the "val" slot.
	//
	Class_ Bool_class =
	basic_class_(constants.Bool, constants.Object, single_Features(attr(constants.val, constants.prim_slot, no_expr())),filename);
	
	//
	// The class Str has a number of slots and operations:
	//       val                                  the length of the string
	//       str_field                            the string itself
	//       length() : Int                       returns length of the string
	//       concat(arg: Str) : Str               performs string concatenation
	//       substr(arg: Int, arg2: Int): Str     substring selection
	//       
	Class_ Str_class =
	basic_class_(constants.Str, 
			 constants.Object,
		  append_Features(
			  append_Features(
				  append_Features(
					  append_Features(
						  single_Features(attr(constants.val, constants.Int, no_expr())),
											single_Features(attr(constants.str_field, constants.prim_slot, no_expr()))),
										single_Features(method(constants.length, nil_Formals(), constants.Int, no_expr()))),
									single_Features(method(constants.concat, 
																  single_Formals(formal(constants.arg, constants.Str)),
																  constants.Str, 
										  no_expr()))),
								single_Features(method(constants.substr, 
															  append_Formals(single_Formals(formal(constants.arg, constants.Int)), 
																				  single_Formals(formal(constants.arg2, constants.Int))),
															  constants.Str, 
										 no_expr()))),
			 filename);
	
	return append_Classes(
		append_Classes(
			append_Classes(
				append_Classes(
					single_Classes(Bool_class),
						single_Classes(Object_class)
				),
				single_Classes(Int_class )
			),
			single_Classes(IO_class)
		),
		single_Classes(Str_class)
	);
	
}

