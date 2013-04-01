#include "expression_coder.h"

using namespace cgen;
using namespace cgen::constants;
using namespace cgen::symbol;

ExpressionCoder::ExpressionCoder(Expression expr) : expr_(expr) {}

void ExpressionCoder::code_assign(ostream &s){
	assign_class * assign = (assign_class *)expr_;
	
	ExpressionCoder(assign->get_expr()).code(s);
	
	IdentifierAccess & acc = *(IdentifierAccessTable::getInstance().lookup(assign->get_name()));
	
	switch (acc.access_) {
		case IdentifierAccess::LOCAL:
			Emitter::store(ACC,acc.fpOffset_,FP,s);
			break;
		case IdentifierAccess::PARAMETER:
			Emitter::store(ACC,acc.fpOffset_,FP,s);
			break;
		case IdentifierAccess::MEMBER:
			Emitter::store(ACC,acc.fpOffset_ + DEFAULT_OBJFIELDS ,SELF,s);
			break;
		default: break;
	}
}

void ExpressionCoder::code_static_dispatch(ostream &s){
	static_dispatch_class * dispatch = (static_dispatch_class *)expr_;
	CgenNode * class_ = NULL;
	Symbol type = dispatch->get_type_name();
	
	if (type == GlobalTables::getInstance().get_constants().SELF_TYPE) {
		class_ = CurrentCoding::getInstance().get_current_class();
	} else {
		class_ = CgenNodesTable::getInstance().findNode(type);
	}
	
	Emitter::push(FP,s);
	Emitter::push(SELF,s);
	
	Expressions actuals = dispatch->get_actuals();
	for (int i = actuals->first(); actuals->more(i); i = actuals->next(i)){
		ExpressionCoder(actuals->nth(i)).code(s);
		Emitter::push(ACC,s);
	}
	
	ExpressionCoder(dispatch->get_expr()).code(s);
	
	//check for dispath on void
	int not_void_label = LabelMgr::getInstance().nextLabel();
	CgenStringEntry entry(*((StringEntry *)class_->get_filename()));
	Emitter::check_dispatch_on_void(not_void_label, dispatch->get_line_number(), entry, s);
	
	Emitter::move(SELF,ACC,s);
	
	CgenMethod * method = class_->findMethod(dispatch->get_name());
	int method_offset = method->get_offset();
	
	Emitter::partial_load_address(T1,s); Emitter::dispatch_table_ref(class_->get_name(), s); s << endl;
	Emitter::load(T1,method_offset,T1,s);
	Emitter::jalr(T1,s);
	
	Emitter::pop(SELF,s);
	Emitter::pop(FP,s);
	
}

void ExpressionCoder::code_dispatch(ostream &s){
	dispatch_class * dispatch = (dispatch_class *)expr_;
	CgenNode * class_ = NULL;
	Symbol e0_type = dispatch->get_expr()->get_type();
	
	if (e0_type == GlobalTables::getInstance().get_constants().SELF_TYPE) {
		class_ = CurrentCoding::getInstance().get_current_class();
	} else {
		class_ = CgenNodesTable::getInstance().findNode(e0_type);
	}
	
	Emitter::push(FP,s);
	Emitter::push(SELF,s);
	
	Expressions actuals = dispatch->get_actuals();
	
	for (int i = actuals->first(); actuals->more(i); i = actuals->next(i)){
 		ExpressionCoder(actuals->nth(i)).code(s);
		Emitter::push(ACC,s);
	}
	
	ExpressionCoder(dispatch->get_expr()).code(s);
	
	//check for dispath on void
	int not_void_label = LabelMgr::getInstance().nextLabel();
	CgenStringEntry entry(*((StringEntry *)class_->get_filename()));
	Emitter::check_dispatch_on_void(not_void_label, dispatch->get_line_number(), entry, s);
	
	Emitter::move(SELF,ACC,s);
	
	CgenMethod * method = class_->findMethod(dispatch->get_name());
	
	int method_offset = method->get_offset();
	
	Emitter::load(T1,DISPTABLE_OFFSET,SELF,s);
	Emitter::load(T1,method_offset,T1,s);
	Emitter::jalr(T1,s);
	
	Emitter::pop(SELF,s);
	Emitter::pop(FP,s);
	
}

void ExpressionCoder::code_cond(ostream &s){
	cond_class * cond = (cond_class *)expr_;
	
	ExpressionCoder(cond->get_pred()).code(s);
	
	int then_branch = LabelMgr::getInstance().nextLabel();
	int end_if = LabelMgr::getInstance().nextLabel();
	
	Emitter::load_bool_val(T1,BoolConst::true_(),s);	
	Emitter::load(ACC,3,ACC,s);
	Emitter::beq(ACC,T1,then_branch,s);
	
	ExpressionCoder(cond->get_else()).code(s);
	Emitter::branch(end_if,s);
	
	Emitter::label_def(then_branch,s);
	ExpressionCoder(cond->get_then()).code(s);
	Emitter::label_def(end_if,s);

}

void ExpressionCoder::code_loop(ostream &s){
	loop_class * loop = (loop_class *)expr_;
	
	int loop_beggining = LabelMgr::getInstance().nextLabel();
	int done = LabelMgr::getInstance().nextLabel();
	
	Emitter::label_def(loop_beggining,s);
	
	ExpressionCoder(loop->get_pred()).code(s);
	
	Emitter::load_bool(T1,BoolConst::false_(),s);
	Emitter::load(T1,3,T1,s),
	Emitter::load(ACC,3,ACC,s);
	
	Emitter::beq(ACC,T1,done,s);
	
	ExpressionCoder(loop->get_body()).code(s);
	
	Emitter::branch(loop_beggining,s);
	Emitter::label_def(done,s);
	
	Emitter::load_imm(ACC,0,s);
}

void ExpressionCoder::code_typcase(ostream &s){
	typcase_class * case_ = (typcase_class *)expr_;
	IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	
	//Code case expr
	ExpressionCoder(case_->get_expr()).code(s);
	
	//Check dispatch on void
	int not_void_label = LabelMgr::getInstance().nextLabel();
	CgenNode * class_ = CurrentCoding::getInstance().get_current_class();
	CgenStringEntry entry(*((StringEntry *)class_->get_filename()));
	Emitter::check_case_on_void(not_void_label, case_->get_line_number(), entry, s);
	
	//Code case branches
	Cases cases = case_->get_cases();
	branch_class * cur_branch = NULL;
	int offset =-(acc_table.increaseFPPosition());
	Emitter::store(ACC,offset,FP,s);
	
	Emitter::jump_to_branch_resolver(case_->get_index(),s);
	Emitter::jump_to_proper_branch(s);
	
	for (int i = cases->first(); cases->more(i); i = cases->next(i)){
		cur_branch = (branch_class *)cases->nth(i);
		
		Emitter::case_branch_ref(case_->get_index(),cur_branch->get_index(),s); s << LABEL;
		
		acc_table.enterscope();
		Symbol id = cur_branch->get_name();
		acc_table.addid(id, new IdentifierAccess(IdentifierAccess::LOCAL, offset));
	
		ExpressionCoder(cur_branch->get_expr()).code(s);
		Emitter::jump_to_case_end(case_->get_index(),s);
		
		acc_table.exitscope();
	}
	acc_table.decreaseFPPosition();
	
	Emitter::case_end_ref(case_->get_index(),s); s << LABEL;
	
}

void ExpressionCoder::code_block(ostream &s){
	Expressions body = ((block_class *)expr_)->get_body();
	for (int i = body->first(); body->more(i); i = body->next(i)){
		ExpressionCoder(body->nth(i)).code(s);
	}
}

void ExpressionCoder::code_let(ostream &s){
	IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	let_class * let = (let_class *)expr_;
	
	if (let->get_init()->get_expr_type() == Expression_class::NO_EXPR) {
		Emitter::partial_load_address(ACC, s);
		Emitter::default_value_ref(let->get_type_decl(), s);
	} else {
		ExpressionCoder(let->get_init()).code(s);
	}
	
	acc_table.enterscope();
	Symbol id = let->get_identifier();
	int offset =-(acc_table.increaseFPPosition());
	acc_table.addid(id, new IdentifierAccess(IdentifierAccess::LOCAL, offset));
	
	Emitter::store(ACC,offset,FP,s);
	
	ExpressionCoder(let->get_body()).code(s);
	
	acc_table.decreaseFPPosition();
	acc_table.exitscope();
	
}

void ExpressionCoder::code_plus(ostream &s){
	plus_class * plus = (plus_class *)expr_;
	
	ExpressionCoder(plus->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(plus->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	Emitter::fetch_int(T1,T1,s),
	Emitter::fetch_int(T2,ACC,s);
	
	Emitter::add(T1,T1,T2,s);
	Emitter::new_int(T1, s);
	
}

void ExpressionCoder::code_sub(ostream &s){
	sub_class * sub = (sub_class *)expr_;
	
	ExpressionCoder(sub->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(sub->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	Emitter::fetch_int(T1,T1,s),
	Emitter::fetch_int(T2,ACC,s);
	
	Emitter::sub(T1,T1,T2,s);
	Emitter::new_int(T1, s);	
}

void ExpressionCoder::code_mul(ostream &s){
	mul_class * mul = (mul_class *)expr_;
	
	ExpressionCoder(mul->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(mul->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	Emitter::fetch_int(T1,T1,s),
	Emitter::fetch_int(T2,ACC,s);
	
	Emitter::mul(T1,T1,T2,s);
	Emitter::new_int(T1, s);	
}

void ExpressionCoder::code_divide(ostream &s){
	divide_class * divide = (divide_class *)expr_;
	
	ExpressionCoder(divide->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(divide->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	Emitter::fetch_int(T1,T1,s),
	Emitter::fetch_int(T2,ACC,s);
	
	Emitter::div(T1,T1,T2,s);
	Emitter::new_int(T1, s);
	
}

void ExpressionCoder::code_neg(ostream &s){
	neg_class * neg = (neg_class *)expr_;
	
	ExpressionCoder(neg->get_ex()).code(s);
	
	Emitter::load(T1,3,ACC,s);
	Emitter::neg(T1,T1,s);
	Emitter::new_int(T1,s);
	
}

void ExpressionCoder::code_lt(ostream &s){
	lt_class * lt_ = (lt_class *)expr_;
	
	ExpressionCoder(lt_->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(lt_->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	Emitter::load(ACC,3,ACC,s);
	Emitter::load(T1,3,T1,s);
	
	int lt = LabelMgr::getInstance().nextLabel();
	int done = LabelMgr::getInstance().nextLabel();
	
	Emitter::blt(T1,ACC,lt,s);
	Emitter::load_bool(ACC,BoolConst::false_(),s);
	Emitter::branch(done,s);
	Emitter::label_def(lt,s);
	Emitter::load_bool(ACC,BoolConst::true_(),s);
	Emitter::label_def(done,s);
}

void ExpressionCoder::code_eq(ostream &s){
	eq_class * eq = (eq_class *)expr_;
	::Constants & c = GlobalTables::getInstance().get_constants();
	
	ExpressionCoder(eq->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(eq->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	Emitter::move(T2,ACC,s);
	
	Symbol ex_type = eq->get_ex1()->get_type();
	if ( ex_type == c.Int || ex_type == c.Bool || ex_type == c.Str ) {
		Emitter::test_basic_equality(s);
		
	} else {
		
		int equal = LabelMgr::getInstance().nextLabel();
		int done = LabelMgr::getInstance().nextLabel();
		
		Emitter::beq(T1,T2,equal,s);
		Emitter::load_bool(ACC,BoolConst::false_(),s);
		Emitter::branch(done,s);
		Emitter::label_def(equal,s);
		Emitter::load_bool(ACC,BoolConst::true_(),s);
		Emitter::label_def(done,s);
	}
	
}

void ExpressionCoder::code_leq(ostream &s){
	leq_class * leq = (leq_class *)expr_;
	
	ExpressionCoder(leq->get_ex1()).code(s);
	Emitter::push(ACC,s);
	
	ExpressionCoder(leq->get_ex2()).code(s);
	Emitter::pop(T1,s);
	
	//load values
	Emitter::load(ACC,3,ACC,s);
	Emitter::load(T1,3,T1,s);
	
	int less_or_equal = LabelMgr::getInstance().nextLabel();
	int done = LabelMgr::getInstance().nextLabel();
	
	//ojo! alerta amb l'ordre!!!
	Emitter::bleq(T1,ACC,less_or_equal,s);
	Emitter::load_bool(ACC,BoolConst::false_(),s);
	Emitter::branch(done, s);
	Emitter::label_def(less_or_equal,s);
	Emitter::load_bool(ACC,BoolConst::true_(),s);
	Emitter::label_def(done,s);
	
}

void ExpressionCoder::code_comp(ostream &s){
	ExpressionCoder(((comp_class *)expr_)->get_ex()).code(s);
	
	int load_true = LabelMgr::getInstance().nextLabel();
	int done = LabelMgr::getInstance().nextLabel();
	
	Emitter::load_bool_val(T1,BoolConst::false_(),s);
	Emitter::load(ACC,3,ACC,s);
	
	Emitter::beq(T1,ACC,load_true,s);
	Emitter::load_bool(ACC, BoolConst::false_(),s);
	Emitter::branch(done, s);
	Emitter::label_def(load_true, s);
	Emitter::load_bool(ACC,BoolConst::true_(),s);
	Emitter::label_def(done,s);
	
	//if acc = false then load_true else load_false endif
}

void ExpressionCoder::code_int_const(ostream &s){
	static IntTable & table = GlobalTables::getInstance().inttable;
	Symbol token = ((int_const_class *)expr_)->get_token();
	IntEntry * entry = table.lookup_string(token->get_string());
	Emitter::load_int(ACC,CgenIntEntry(*entry),s);
}

void ExpressionCoder::code_bool_const(ostream &s){
	bool value = ((bool_const_class *)expr_)->get_val();
	Emitter::load_bool(ACC, BoolConst::getProperBool(value), s);
}

void ExpressionCoder::code_string_const(ostream &s){
	static StrTable stringtable = GlobalTables::getInstance().stringtable;
	Symbol token = ((string_const_class *)expr_)->get_token();
	
	CgenStringEntry entry(*(stringtable.lookup_string(token->get_string())));
	Emitter::load_string(ACC,entry,s);
}

void ExpressionCoder::code_new(ostream &s){
	static Symbol SELF_ = GlobalTables::getInstance().get_constants().SELF_TYPE;
	Symbol class_name = ((new__class *)expr_)->get_type_name();
	
	if (class_name == SELF_) {
		
		//Load protoObj reference and create new object. Pushes table index into the stack
		Emitter::load_address(ACC, CLASSOBJTAB, s);
		Emitter::load(T1,TAG_OFFSET,SELF,s);
		Emitter::load_imm(T2,WORDS_PER_CLASS_OBJ_ENTRY * WORD_SIZE,s); 
 		Emitter::mul(T1,T1,T2,s);
		Emitter::add(ACC,T1,ACC,s);
		Emitter::addiu(ACC,ACC,CLASS_OBJ_OFFSET_PROTOBJ * WORD_SIZE,s);
		Emitter::load(ACC,0,ACC,s);
		Emitter::push(T1,s);
		
		Emitter::jal("Object.copy",s);
		
		Emitter::pop(T1,s);
		Emitter::push(ACC,s); //Since the initialization can override the pointer to the new object, it is saved
		
		//Load address of initialization method, and jump to it
		Emitter::load_address(T2, CLASSOBJTAB, s);
		Emitter::add(T2,T2,T1,s);
		Emitter::addiu(T2,T2,CLASS_OBJ_OFFSET_INIT*WORD_SIZE,s);
		Emitter::load(T2,0,T2,s);
		
		//Common arguments saving
		Emitter::push(FP,s);
		Emitter::push(SELF,s);
		Emitter::move(SELF,ACC,s);
		
		Emitter::jalr(T2,s);
		
		Emitter::pop(SELF,s);
		Emitter::pop(FP,s);
		
	} else {		
		Emitter::new_(class_name,s);
	}
	
	Emitter::pop(ACC,s);	
	
}

void ExpressionCoder::code_isvoid(ostream &s){
	ExpressionCoder(((isvoid_class *)expr_)->get_ex()).code(s);
	
	int is_void_true = LabelMgr::getInstance().nextLabel();
	int end_check = LabelMgr::getInstance().nextLabel();
	
	Emitter::beqz(ACC, is_void_true, s);
	Emitter::load_bool(ACC, BoolConst::false_(), s);
	Emitter::branch(end_check, s);
	Emitter::label_def(is_void_true, s);
	Emitter::load_bool(ACC, BoolConst::true_(), s);
	Emitter::label_def(end_check, s);
}

void ExpressionCoder::code_no_expr(ostream &s){
	
}

void ExpressionCoder::code_obj(ostream &s){
	IdentifierAccessTable & acc_table = IdentifierAccessTable::getInstance();
	
	object_class * obj = (object_class *)expr_;
	
	IdentifierAccess & acc = *(acc_table.lookup(obj->get_name()));
	
	switch (acc.access_) {
		case IdentifierAccess::LOCAL:
			Emitter::load(ACC,acc.fpOffset_,FP,s);
			break;
		case IdentifierAccess::PARAMETER:
			Emitter::load(ACC,acc.fpOffset_,FP,s);
			break;
		case IdentifierAccess::MEMBER:
			Emitter::load(ACC,acc.fpOffset_ + DEFAULT_OBJFIELDS ,SELF,s);
			break;
		case IdentifierAccess::SELF:
			Emitter::move(ACC,SELF,s);
		break;
	}
}

void ExpressionCoder::code(ostream & s) {
	switch (expr_->get_expr_type()) {
		case Expression_class::ASSIGN:
			code_assign(s);
			break;
		case Expression_class::STATIC_DISPATCH:
			code_static_dispatch(s);
			break;
		case Expression_class::DISPATCH:
			code_dispatch(s);
			break;
		case Expression_class::COND:
			code_cond(s);
			break;
		case Expression_class::LOOP:
			code_loop(s);
			break;
		case Expression_class::TYPCASE:
			code_typcase(s);
			break;
		case Expression_class::BLOCK:
			code_block(s);
			break;
		case Expression_class::LET:
			code_let(s);
			break;
		case Expression_class::PLUS:
			code_plus(s);
			break;
		case Expression_class::SUB:
			code_sub(s);
			break;
		case Expression_class::MUL:
			code_mul(s);
			break;
		case Expression_class::DIVIDE:
			code_divide(s);
			break;
		case Expression_class::NEG:
			code_neg(s);
			break;
		case Expression_class::LT:
			code_lt(s);
			break;
		case Expression_class::EQ:
			code_eq(s);
			break;
		case Expression_class::LEQ:
			code_leq(s);
			break;
		case Expression_class::COMP:
			code_comp(s);
			break;
		case Expression_class::INT_CONST:
			code_int_const(s);
			break;
		case Expression_class::BOOL_CONST:
			code_bool_const(s);
			break;
		case Expression_class::STRING_CONST:
			code_string_const(s);
			break;
		case Expression_class::NEW:
			code_new(s);
			break;
		case Expression_class::ISVOID:
			code_isvoid(s);
			break;
		case Expression_class::NO_EXPR:
			code_no_expr(s);
			break;
		case Expression_class::OBJ:
			code_obj(s);
			break;
	}
}

int ExpressionCoder::collect_tree_info_in_assign(){
	return ExpressionCoder(((assign_class *)expr_)->get_expr()).collectTreeInfo();
}

int ExpressionCoder::collect_tree_info_in_static_dispatch(){
	static_dispatch_class * dispatch = (static_dispatch_class *)expr_;
	int max = ExpressionCoder((dispatch->get_expr())).collectTreeInfo();
	int temp = 0;
	Expressions actuals = dispatch->get_actuals();
	
	for (int i = actuals->first(); actuals->more(i); i = actuals->next(i)){
		temp = ExpressionCoder(actuals->nth(i)).collectTreeInfo();
		max = (temp > max ) ?  temp : max ; 
	}
	
	return max;
}

int ExpressionCoder::collect_tree_info_in_dispatch(){
	dispatch_class * dispatch = (dispatch_class *)expr_;
	int max = ExpressionCoder((dispatch->get_expr())).collectTreeInfo();
	int temp = 0;
	Expressions actuals = dispatch->get_actuals();
	
	for (int i = actuals->first(); actuals->more(i); i = actuals->next(i)){
		temp = ExpressionCoder(actuals->nth(i)).collectTreeInfo();
		max = (temp > max ) ?  temp : max ; 
	}
	
	return max;
}

int ExpressionCoder::collect_tree_info_in_cond(){
	cond_class * cond = (cond_class *)expr_;
	int temp = 0;
	int max = ExpressionCoder(cond->get_pred()).collectTreeInfo();
	temp = ExpressionCoder(cond->get_then()).collectTreeInfo();
	max = ( temp > max ) ? temp : max ;
	temp = ExpressionCoder(cond->get_else()).collectTreeInfo();
	return ( temp > max ) ? temp : max ;
	
}

int ExpressionCoder::collect_tree_info_in_loop(){
	loop_class * loop = (loop_class *)expr_;
	int max = ExpressionCoder(loop->get_pred()).collectTreeInfo();
	int temp = ExpressionCoder(loop->get_body()).collectTreeInfo();
	return ( temp > max ) ? temp : max ;
}

int ExpressionCoder::collect_tree_info_in_typcase(){
	//Three things have to be done here: first, count normals -as usual-.
	//Second, this expression has to be recorded, given also and index to it
	//Third, and index is given to all the case branches
	
	typcase_class * case_ = (typcase_class *)expr_;
	int max = ExpressionCoder(case_->get_expr()).collectTreeInfo();
	
 	int loop_temp = 0;
	int temp = 0;
	
	Cases cases = case_->get_cases();
	int ind = 0;
	
	for (int i = cases->first(); cases->more(i); i = cases->next(i)){
		Case_class * branch = cases->nth(i);
		branch->set_index(ind);
		ind++;
		
		loop_temp = ExpressionCoder(branch->get_expr()).collectTreeInfo();
		
		if (loop_temp > temp) temp = loop_temp;
	}
	
	CasesTable & cases_tab = CasesTable::getInstance();
	cases_tab.addCase(*case_);
	case_->set_index(cases_tab.get_next_case_index());
	case_->set_branches_count(ind);
	
	return (( temp > max ) ? temp : max ) + 1;
}

int ExpressionCoder::collect_tree_info_in_block(){
	Expressions body =((block_class *)expr_)->get_body();
	int max = 0;
	int temp = 0;
	
	for (int i = body->first(); body->more(i); i = body->next(i)){
		temp = ExpressionCoder(body->nth(i)).collectTreeInfo();
		max = (temp > max ) ? temp : max ;
	}
	
	return max;
}

int ExpressionCoder::collect_tree_info_in_let(){
	let_class * let = (let_class *)expr_;
	int max = ExpressionCoder(let->get_init()).collectTreeInfo();
	int temp = ExpressionCoder(let->get_body()).collectTreeInfo();
	
	return (( temp > max ) ? temp : max) + 1;
}

int ExpressionCoder::collect_tree_info_in_binary_arithmetic(){
	BinaryArithmeticExpression * expr = (BinaryArithmeticExpression *)expr_;
	
	int max = ExpressionCoder(expr->get_ex1()).collectTreeInfo();
	int temp = ExpressionCoder(expr->get_ex2()).collectTreeInfo();
	
	return ( temp > max ) ? temp : max ;
}


int ExpressionCoder::collect_tree_info_in_binary_relational(){
	BinaryRelationalExpression * expr = (BinaryRelationalExpression *)expr_;
	
	int max = ExpressionCoder(expr->get_ex1()).collectTreeInfo();
	int temp = ExpressionCoder(expr->get_ex2()).collectTreeInfo();
	
	return ( temp > max ) ? temp : max ;
}

int ExpressionCoder::collect_tree_info_in_neg() {
	return ExpressionCoder(((neg_class *)expr_)->get_ex()).collectTreeInfo();
}

int ExpressionCoder::collect_tree_info_in_eq(){
	eq_class * expr = (eq_class *)expr_;
	
	int max = ExpressionCoder(expr->get_ex1()).collectTreeInfo();
	int temp = ExpressionCoder(expr->get_ex2()).collectTreeInfo();
	
	return ( temp > max ) ? temp : max ;
}

int ExpressionCoder::collect_tree_info_in_comp(){
	return ExpressionCoder(((comp_class *)expr_)->get_ex()).collectTreeInfo();
}

int ExpressionCoder::collect_tree_info_in_isvoid(){
	return ExpressionCoder(((isvoid_class *)expr_)->get_ex()).collectTreeInfo();	
}


int ExpressionCoder::collectTreeInfo() {
	int ret = 0;
	switch (expr_->get_expr_type()) {
		case Expression_class::ASSIGN:
			ret = collect_tree_info_in_assign();
			break;
		case Expression_class::STATIC_DISPATCH:
			ret = collect_tree_info_in_static_dispatch();
			break;
		case Expression_class::DISPATCH:
			ret = collect_tree_info_in_dispatch();
			break;
		case Expression_class::COND:
			ret = collect_tree_info_in_cond();
			break;
		case Expression_class::LOOP:
			ret = collect_tree_info_in_loop();
			break;
		case Expression_class::TYPCASE:
			ret = collect_tree_info_in_typcase();
			break;
		case Expression_class::BLOCK:
			ret = collect_tree_info_in_block();
			break;
		case Expression_class::LET:
			ret = collect_tree_info_in_let();
			break;
		case Expression_class::NEG:
			ret = collect_tree_info_in_neg();
			break;
		case Expression_class::EQ:
			ret = collect_tree_info_in_eq();
			break;
		case Expression_class::COMP:
			ret = collect_tree_info_in_comp();
			break;
		case Expression_class::ISVOID:
			ret = collect_tree_info_in_isvoid();
			break;
		case Expression_class::PLUS:
		case Expression_class::SUB:
		case Expression_class::MUL:
		case Expression_class::DIVIDE:
			ret = collect_tree_info_in_binary_arithmetic();
			break;
		case Expression_class::LT:
		case Expression_class::LEQ:
			ret = collect_tree_info_in_binary_relational();
			break;
		default: break;
	}	
	return ret;
}