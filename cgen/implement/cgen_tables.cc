#include "cgen_tables.h"

using namespace cgen;
using namespace cgen::symbol;

CodifierStrTable::CodifierStrTable(StrTable & tbl, int class_tag) : tbl_(&tbl), class_tag_(class_tag) {}

void CodifierStrTable::code(ostream & str){
	for (int i = tbl_->first(); tbl_->more(i); i = tbl_->next(i)){
		CgenStringEntry(*(tbl_->lookup(i))).code_def(str, class_tag_);
	}
}

CodifierIntTable::CodifierIntTable(IntTable & tbl, int class_tag) : tbl_(&tbl), class_tag_(class_tag) {}

void CodifierIntTable::code(ostream & str) {
	for (int i = tbl_->first(); tbl_->more(i); i = tbl_->next(i)){
		CgenIntEntry(*(tbl_->lookup(i))).code_def(str, class_tag_);
	}
}

CgenNodesTable::CgenNodesTable() : first_node_(NULL), last_node_(NULL) {}

CgenNodesTable & CgenNodesTable::getInstance() {
	static CgenNodesTable S;
	return S;
}

BasicListIterator<CgenNode> * CgenNodesTable::getIterator() {
	return first_node_->getIterator();
}

CgenNode * CgenNodesTable::findNode(Symbol name){
	BasicListIterator<CgenNode> * iter = getIterator();
	bool found = false;
	
	CgenNode * ret = NULL;
	CgenNode * temp = NULL;
	while (!found && iter->more()){
		temp = iter->current();
		
		if (temp->get_name() == name){
			found = true;
			ret = temp;
		}
		iter->move();
	}
	
	return ret;
}

void CgenNodesTable::addNode(CgenNode & nd){
	if (!first_node_) {
		first_node_ = new List<CgenNode>(&nd,NULL);
		last_node_ = first_node_;
	} else {
		last_node_->add(nd);
		last_node_ = last_node_->tl();
	}
	
}

MethodsOverridingTable::MethodsOverridingTable() {
	enterscope();
}

MethodsOverridingTable & MethodsOverridingTable::getInstance() {
	static MethodsOverridingTable M;
	return M;
}

IdentifierAccess::IdentifierAccess(IDENTIFER_ACCESS access) : IdentifierAccess(access,0) {}
IdentifierAccess::IdentifierAccess(IDENTIFER_ACCESS access, int fpOffset) : access_(access), fpOffset_(fpOffset) {}

IdentifierAccessTable::IdentifierAccessTable() : FP_position_(-1) { enterscope(); }

IdentifierAccessTable & IdentifierAccessTable::getInstance() {
	static IdentifierAccessTable I;
	return I;
}

int IdentifierAccessTable::increaseFPPosition(){
	return ++FP_position_;
}

void IdentifierAccessTable::decreaseFPPosition() {
	FP_position_--;
}

void IdentifierAccessTable::resetFPPosition() {
	FP_position_ = -1;
}
