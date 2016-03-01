#include <basic.h>
#include <cool-tree.h>

//#include "cool-parse.h" <<-esborrar aquest fitxer
#include "cool.tab.hh"
#include "LexerWrapper.h"


namespace parsing {
	
	class Results {
	private:
		Program root;
		Classes classes;        /* for use in semantic analysis */
		int errorsCount;
		
	public:
		Results();
		
		void set_root(Program p);
		Program get_root();
		
		void set_classes(Classes cls);
		Classes get_classes();
		
		int get_errorsCount();
		void incErrorsCount();
	};
	
	class ParserDriver {
		
	private:
		static const int MAX_ERRORS_ALLOWED = 50;
		
		Lexer lexer;
		Parser parser;
		Results *_results;
		char * filename;
		
	public:
		ParserDriver(flags::FlagState debugLexer, flags::FlagState debugParser, char * _filename);
		
		int parse();
		
		char * get_filename();
		void check_error_count();
		
		Results * results();
		
		void update_curr_lineno(int line);
		int get_curr_lineno();

	};
	
	class TokenHelper {
	private:
		Parser::token_type token;
		Parser::semantic_type val;
	public:
		TokenHelper(Parser::token_type token);
		TokenHelper(Parser::token_type token, Parser::semantic_type yylval);
		
		
		/**Given a string representation OF A KEYWORD -which also cannot be true or false-, returns its token*/
		static Parser::token_type get_keyword_token(char *str);
		
		char const * toString();
		void dump(ostream& out, int lineno);
	};
	
}
