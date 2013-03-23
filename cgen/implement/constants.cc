#include "constants.h"

using namespace cgen;

Constants::Constants() : next_class_tag_(-1), obj_class_tag_(-1), int_class_tag_(-1), io_class_tag_(-1), \
	bool_class_tag_(-1), str_class_tag_(-1) { }

Constants & Constants::getInstance() {
	static Constants C;
	return C;
}

int Constants::nextClassTag() {
	return ++next_class_tag_;
}

int Constants::get_base_class_tag(BASE_CLASSES class_){
	switch (class_) {
		case OBJECT: return obj_class_tag_;
		break;
		case INT: return int_class_tag_;
		break;
		case IO: return io_class_tag_;
		break;
		case BOOL: return bool_class_tag_;
		break;
		case STR: return str_class_tag_;
		break;
	}
	return -1;
}

void Constants::set_base_class_tag(BASE_CLASSES class_, int class_tag){
	switch (class_) {
		case OBJECT: obj_class_tag_ = class_tag;
		break;
		case INT: int_class_tag_ = class_tag;
		break;
		case IO: io_class_tag_ = class_tag;
		break;
		case STR: str_class_tag_ = class_tag;
		break;
		case BOOL: bool_class_tag_ = class_tag;
		break;
	}
}

LabelMgr::LabelMgr() {
	cur_label_ = 0;
}

LabelMgr & LabelMgr::getInstance(){
	static LabelMgr L;
	return L;
}

int LabelMgr::nextLabel() {
	return cur_label_++;
}
