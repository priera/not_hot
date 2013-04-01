#include "cgen_node.h"

using namespace cgen;
using namespace cgen::constants;
using namespace cgen::symbol;

CgenAttribute::CgenAttribute(attr_class attr, CgenNode & class_, int offset) : \
	attr_class(attr), class__(&class_), offset_(offset), locals_in_init_(0) {
	
	if (init){
		locals_in_init_ = ExpressionCoder(init).collectTreeInfo();
	}
	
}

int CgenAttribute::get_offset() { return offset_; }
int CgenAttribute::get_locals_in_init() { return locals_in_init_; }

void CgenAttribute::emitDef(ostream & out){
	out << WORD;
	
	Emitter::default_value_ref(type_decl, out);
}

void CgenAttribute::codeInitExpr(ostream & output){
	//Code attribute expression, load self object, and store attribute value in the object
	IdentifierAccessTable::getInstance().resetFPPosition();
	ExpressionCoder(init).code(output);
	
	emitStoreVal(SELF, output);
}

bool CgenAttribute::type_has_default_value() {
	::Constants c = GlobalTables::getInstance().get_constants();
	
	return (type_decl == c.Int || type_decl == c.Str || type_decl == c.Bool);
}

void CgenAttribute::codeInitialization(ostream & output){
	if (has_init_expr()) {
		codeInitExpr(output);
	} 
}

void CgenAttribute::emitStoreVal(std::string self_reg, ostream & s){
	Emitter::store(ACC, offset_ + DEFAULT_OBJFIELDS, self_reg, s);
}

CgenMethod::CgenMethod(method_class m, CgenNode & defining_class, int offset) : method_class(m), defining_class_(&defining_class), offset_(offset), locals_count_(0) {
	locals_count_ = ExpressionCoder(expr).collectTreeInfo();
}

void CgenMethod::registerArguments(){
	IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	acc_table.resetFPPosition();
	int arg_offset = 1;
	Formals reversed_arguments = formals->reverse();
	
	for (int i = reversed_arguments->first(); reversed_arguments->more(i); i = reversed_arguments->next(i)){

		IdentifierAccess * acc = new IdentifierAccess(IdentifierAccess::PARAMETER,arg_offset);
		
		acc_table.addid(reversed_arguments->nth(i)->get_name(), acc);	
		arg_offset++;
	} 
	
}

int CgenMethod::get_offset() { return offset_; }
void CgenMethod::set_offset(int offset) { offset_ = offset; }
CgenNode & CgenMethod::get_defining_class() { return *defining_class_; }


void CgenMethod::code(ostream & output){
	
	IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	
	acc_table.enterscope();
	
	registerArguments();
 	
	Emitter::method_ref(defining_class_->get_name(), name,output); output << ":" << endl;
	
	Emitter::callee_begin(locals_count_, output);
	
	ExpressionCoder(expr).code(output);
	
	Emitter::callee_return(formals->len(), locals_count_, output);
	
	acc_table.exitscope();
	
}

bool CgenMethod::is_basic() {
	return defining_class_->is_basic_class();
}

CgenNode::CgenNode(class__class c, CgenNode * parent) : class__class(c), parentnd_(parent), cgen_children_(NULL), attributes_(NULL), own_attributes_(NULL), methods_(NULL), \
	own_methods_(NULL), overridden_methods_(NULL), classtag_(-1), size_(-1), attr_count_(0), method_count_(0)
	{
		set_classtag();
		
		MethodsOverridingTable::getInstance().enterscope();
		
		collect_features();
		set_object_size();
		
		CgenNodesTable::getInstance().addNode(*this);
		
		gather_children();
		
		MethodsOverridingTable::getInstance().exitscope();
	}

void CgenNode::set_classtag() {
	classtag_ = Constants::getInstance().nextClassTag();
	
	if (!is_basic_class_) return;
	
	::Constants c = GlobalTables::getInstance().get_constants();
	if (name == c.Object) {
		Constants::getInstance().set_base_class_tag(Constants::OBJECT, classtag_);
	} else if (name == c.Int) {
		Constants::getInstance().set_base_class_tag(Constants::INT, classtag_);
	} else if (name == c.Bool) {
		Constants::getInstance().set_base_class_tag(Constants::BOOL, classtag_);
	} else if (name == c.IO) {
		Constants::getInstance().set_base_class_tag(Constants::IO, classtag_);
	} else if (name == c.Str) {
		Constants::getInstance().set_base_class_tag(Constants::STR, classtag_);
	}
}

List<CgenNode> * CgenNode::get_children_nodes() {
	return cgen_children_;
}

void CgenNode::set_locals_in_constructor() {
	int locals = 0;

	BasicListIterator<CgenAttribute> * iter = own_attributes_->getIterator();
	CgenAttribute * attr;
	while (iter->more()) {
		attr = iter->current();
		locals = (attr->get_locals_in_init() > locals ) ? attr->get_locals_in_init() : locals ;
		
		iter->move();
	}
	
	locals_in_constructor_ = locals;
	
}

void CgenNode::collect_features() {
	
	if (parentnd_ != NULL ){
		methods_ = parentnd_->methods_->copy();
		attributes_ = parentnd_->attributes_;
		attr_count_ += parentnd_->attr_count_;
		method_count_ += parentnd_->method_count_;
	}
	
	own_attributes_ = NULL;
	AttributesIterator iter_attr = getAttributesIterator();
	while (iter_attr.more()){
		CgenAttribute * attr = new CgenAttribute(*(iter_attr.current()),*this, attr_count_);
		attr_count_++;
		own_attributes_ = new List<CgenAttribute>(attr, own_attributes_);
		iter_attr.move();
	}
		
	own_attributes_ = own_attributes_->reverse();
	attributes_ = List<CgenAttribute>::concat(attributes_, own_attributes_->copy());
	set_locals_in_constructor();
	
	MethodsOverridingTable & table = MethodsOverridingTable::getInstance();
	
	own_methods_ = NULL;
	MethodsIterator iter_met = getMethodsIterator();
	CgenMethod * base_method = NULL;
		
	CgenMethod * met = NULL;
	while (iter_met.more()){
		method_class * cur = iter_met.current();
		base_method = table.lookup(cur->get_name());
		met = NULL;
		
		if ( base_method != NULL) {
			int met_offset = base_method->get_offset();
			met = new CgenMethod(*cur,*this,met_offset);
			List<CgenMethod>::replace(methods_, base_method, met);
			overridden_methods_ = new List<CgenMethod>(met, overridden_methods_);
			
		} else {
			met = new CgenMethod(*cur,*this,method_count_);
			own_methods_ = new List<CgenMethod>(met, own_methods_);
			method_count_++;
		}
		
		table.addid(met->get_name(), met);
		iter_met.move();
			
	}
	
	own_methods_ = own_methods_->reverse();		
	methods_ = List<CgenMethod>::concat(methods_, own_methods_->copy());
		
}

void CgenNode::set_object_size() {
	size_ =  3 + attr_count_; 
}

void CgenNode::gather_children(){

	for (int i = children->first(); children->more(i); i = children->next(i)){
		class__class c  = *((class__class *)children->nth(i));
		CgenNode * n = new CgenNode(c, this);
		cgen_children_ = new List<CgenNode>(n, cgen_children_);
	}
}

void CgenNode::emitPrototables(ostream & output){
	output << WORD << "-1" << endl;
	Emitter::protobj_ref(name, output); output << LABEL;
	output << WORD << classtag_ << endl;
	output << WORD << size_ << endl;
	output << WORD ; Emitter::dispatch_table_ref(name, output); output << endl;
	
	for (List<CgenAttribute> *attr = attributes_; attr; attr = attr->tl()){
		attr->hd()->emitDef(output);	
	}
	
	BasicListIterator<CgenNode> * iter = cgen_children_->getIterator();
	while (iter->more()) {
		iter->current()->emitPrototables(output);
		iter->move();
	}

}

void CgenNode::emitDispatchTableContent(ostream & output){
	
	BasicListIterator<CgenMethod> * iterM = methods_->getIterator();

	while (iterM->more()) {
		CgenMethod cur = *(iterM->current());
		output << WORD; Emitter::method_ref(cur.get_defining_class().get_name(), cur.get_name(), output); output << endl;
		iterM->move();
	}
}

void CgenNode::emitDispatchTable(ostream & output){
	Emitter::dispatch_table_ref(name, output); output << LABEL;
	emitDispatchTableContent(output);

	BasicListIterator<CgenNode> * iter = cgen_children_->getIterator();
	while (iter->more()) {
		iter->current()->emitDispatchTable(output);
		iter->move();
	}

}

void CgenNode::codeInitializer(ostream & output){
	using namespace cgen::constants;
	static IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	
	Emitter::init_ref(name, output); output << cgen::constants::LABEL;
	Emitter::callee_begin(locals_in_constructor_, output);
	
	if (parentnd_ != NULL){
		//jump to parent
		Emitter::push(FP,output);
		output << JAL ; Emitter::init_ref(parentnd_->name, output); output << endl;
		Emitter::pop(FP,output);
	}
	
	BasicListIterator<CgenAttribute> * iter = own_attributes_->getIterator();
	CgenAttribute * attr;
	while (iter->more()) {
		attr = iter->current();

		attr->codeInitialization(output);
		
		iter->move();
	}
	
	Emitter::callee_return(0, locals_in_constructor_, output);

}

void CgenNode::registerAttributes() {
	static IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	
	BasicListIterator<CgenAttribute> * iter = own_attributes_->getIterator();
	CgenAttribute * attr;
	while (iter->more()) {
		attr = iter->current();
		
		acc_table.addid(attr->get_name(), new IdentifierAccess(IdentifierAccess::MEMBER,attr->get_offset()));
		iter->move();
	}
	
}

void CgenNode::codeMethods(ostream & output){
	
	if (!is_basic_class_){
		BasicListIterator<CgenMethod> * iter = overridden_methods_->getIterator();
		while (iter->more()) {
			
			iter->current()->code(output);
			iter->move();
		}
		
		iter = own_methods_->getIterator();
		while (iter->more()) {
			
			iter->current()->code(output);
			iter->move();
		}
	}

}

void CgenNode::code(ostream & output){
	IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	
	CurrentCoding::getInstance().set_current_class(*this);
	acc_table.enterscope();
	acc_table.addid(GlobalTables::getInstance().get_constants().self, new IdentifierAccess(IdentifierAccess::SELF));
	
	registerAttributes();
	
	codeInitializer(output);
	
	codeMethods(output);
		
	BasicListIterator<CgenNode> * iter = cgen_children_->getIterator();
	while (iter->more()) {
		iter->current()->code(output);
		iter->move();
	}
	
	acc_table.exitscope();
}

int CgenNode::get_classtag() {
	return classtag_;
}

CgenNode * CgenNode::get_nd_parent() {
	return parentnd_;
}

CgenMethod * CgenNode::findMethod(Symbol method_name){
	BasicListIterator<CgenMethod> * iter = methods_->getIterator();
	bool found = false;
	CgenMethod * ret = NULL;
	
	while (!found && iter->more()) {
		if (iter->current()->get_name() == method_name){
			found = true;
			ret = iter->current();
		}
		iter->move();
	}
	
	return ret;
}

CurrentCoding::CurrentCoding() : current_class_(NULL) {}

CurrentCoding & CurrentCoding::getInstance() {
	static CurrentCoding C;
	return C;
}

void CurrentCoding::set_current_class(CgenNode & class_){
	current_class_ = &class_;
}

CgenNode * CurrentCoding::get_current_class() {
	return current_class_;
}
