#ifndef CGEN_CONSTANTS_H
#define CGEN_CONSTANTS_H

namespace cgen {
	
	
	//TODO: Should be refactored to "Globals"
	class Constants {
	private:
		int next_class_tag_;
		
		int obj_class_tag_;
		int int_class_tag_;
		int io_class_tag_;
		int bool_class_tag_;
		int str_class_tag_;
		
		Constants();
	public:
		
		enum BASE_CLASSES {
			OBJECT, INT, IO, STR, BOOL
		};
		
		static Constants & getInstance();
		
		int nextClassTag();
		
		int get_base_class_tag(BASE_CLASSES class_);
		void set_base_class_tag(BASE_CLASSES class_, int class_tag);
		
	};
	
	class LabelMgr {
	private:
		int cur_label_;
		
		LabelMgr();
	public:
		static LabelMgr & getInstance();
		int nextLabel();
		
	};
	
} 

#endif