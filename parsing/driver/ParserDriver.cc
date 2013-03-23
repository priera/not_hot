#include "ParserDriver.h"

namespace parsing {
		
	Results::Results() : root(NULL), classes(NULL), errorsCount(0) {
		;
	}
	
	void Results::set_root(Program p){
		root = p;
	}
	
	Program Results::get_root(){
		return root;
	}
	
	void Results::set_classes(Classes cls){
		classes = cls;
	}
	
	Classes Results::get_classes(){
		return classes;
	}
	
	int Results::get_errorsCount(){
		return errorsCount;
	}
	
	void Results::incErrorsCount(){
		errorsCount++;
	}
	
	ParserDriver::ParserDriver(flags::FlagState debugLexer, flags::FlagState debugParser, char * _filename) \
	: parser(*this, lexer), filename(_filename) {
		lexer.set_debug((debugLexer == flags::ACTIVE) ? 1 : 0 );
		parser.set_debug_level((debugParser == flags::ACTIVE) ? 1 : 0 );
		_results = new Results();
	};

	char * ParserDriver::get_filename(){
		return filename;
	}
	
	int ParserDriver::parse(){
		Lineno_Mngr::reset();
		return parser.parse();
	};
	
	void ParserDriver::check_error_count() {
		if (_results->get_errorsCount() > MAX_ERRORS_ALLOWED) {fprintf(stdout, "More than 50 errors\n"); exit(1);}
	}
	
	Results * ParserDriver::results(){
		return _results;
	}
	
	void ParserDriver::update_curr_lineno(int line){
		Lineno_Mngr::curr_lineno = line;
	}
	
	int ParserDriver::get_curr_lineno(){
		return lexer.lineno();
	}

	TokenHelper::TokenHelper(Parser::token_type token) : token(token), val() {};
	TokenHelper::TokenHelper(Parser::token_type token, Parser::semantic_type yylval) : token(token), val(yylval) {};
	
	Parser::token_type TokenHelper::get_keyword_token(char * str){
		std::string s (changeCase(str));
		
		if (s.compare("class") == 0) {
			return Parser::token::CLASS;
		} else if (s.compare("if") == 0) {
			return Parser::token::IF;
		} else if (s.compare("then") == 0) {
			return Parser::token::THEN;
		} else if (s.compare("else") == 0) {
			return Parser::token::ELSE;
		} else if (s.compare("fi") == 0) {
			return Parser::token::FI;
		} else if (s.compare("in") == 0) {	
			return Parser::token::IN;
		} else if (s.compare("inherits") == 0) {
			return Parser::token::INHERITS;
		} else if (s.compare("isvoid") == 0) {
			return Parser::token::ISVOID;
		} else if (s.compare("let") == 0) {
			return Parser::token::LET;
		} else if (s.compare("loop") == 0) {
			return Parser::token::LOOP;
		} else if (s.compare("pool") == 0) {
			return Parser::token::POOL;
		} else if (s.compare("while") == 0) {
			return Parser::token::WHILE;
		} else if (s.compare("case") == 0) {
			return Parser::token::CASE;	
		} else if (s.compare("esac") == 0) {
			return Parser::token::ESAC;
		} else if (s.compare("new") == 0) {
			return Parser::token::NEW;
		} else if (s.compare("of") == 0) {
			return Parser::token::OF;
		} else if (s.compare("not") == 0){
			return Parser::token::NOT;
		}
		
		return Parser::token::ERROR;
	}
	
	char* TokenHelper::toString(){
		switch (token) {
			case 0:            					return("EOF");        break;
			case (Parser::token::CLASS):      return("CLASS");      break;
			case (Parser::token::ELSE):       return("ELSE");       break;
			case (Parser::token::FI):         return("FI");         break;
			case (Parser::token::IF):         return("IF");         break;
			case (Parser::token::IN):         return("IN");         break;
			case (Parser::token::INHERITS):   return("INHERITS");   break;
			case (Parser::token::LET):        return("LET");        break;
			case (Parser::token::LOOP):       return("LOOP");       break;
			case (Parser::token::POOL):       return("POOL");       break;
			case (Parser::token::THEN):       return("THEN");       break;
			case (Parser::token::WHILE):      return("WHILE");      break;
			case (Parser::token::ASSIGN):     return("ASSIGN");     break;
			case (Parser::token::CASE):       return("CASE");       break;
			case (Parser::token::ESAC):       return("ESAC");       break;
			case (Parser::token::OF):         return("OF");         break;
			case (Parser::token::DARROW):     return("DARROW");     break;
			case (Parser::token::NEW):        return("NEW");        break;
			case (Parser::token::STR_CONST):  return("STR_CONST");  break;
			case (Parser::token::INT_CONST):  return("INT_CONST");  break;
			case (Parser::token::BOOL_CONST): return("BOOL_CONST"); break;
			case (Parser::token::TYPEID):     return("TYPEID");     break;
			case (Parser::token::OBJECTID):   return("OBJECTID");   break;
			case (Parser::token::ERROR):      return("ERROR");      break;
			case (Parser::token::LE):         return("LE");         break;
			case (Parser::token::NOT):        return("NOT");        break;
			case (Parser::token::ISVOID):     return("ISVOID");     break;
			case '+': return("'+'"); break;
			case '/': return("'/'"); break;
			case '-': return("'-'"); break;
			case '*': return("'*'"); break;
			case '=': return("'='"); break;
			case '<': return("'<'"); break;
			case '.': return("'.'"); break;
			case '~': return("'~'"); break;
			case ',': return("','"); break;
			case ';': return("';'"); break;
			case ':': return("':'"); break;
			case '(': return("'('"); break;
			case ')': return("')'"); break;
			case '@': return("'@'"); break;
			case '{': return("'{'"); break;
			case '}': return("'}'"); break;
			default:  return("<Invalid Token>");
		}
	}
	
	void TokenHelper::dump(ostream& out, int lineno){
		out << "#" << lineno << " " << toString();
		
		switch (token) {
			case (Parser::token::STR_CONST):
				out << " \"";
				print_escaped_string(out, val.symbol->get_string());
				out << "\"";
				#ifdef CHECK_TABLES
				stringtable.lookup_string(val.symbol->get_string());
				#endif
				break;
			case (Parser::token::INT_CONST):
				out << " " << val.symbol;
				#ifdef CHECK_TABLES
				inttable.lookup_string(val.symbol->get_string());
				#endif
				break;
			case (Parser::token::BOOL_CONST):
				out << (val.boolean ? " true" : " false");
				break;
			case (Parser::token::TYPEID):
			case (Parser::token::OBJECTID):
				out << " " << val.symbol;
				#ifdef CHECK_TABLES
				idtable.lookup_string(val.symbol.get_string());
				#endif
				break;
			case (Parser::token::ERROR): 
				// sm: I've changed assignment 2 so students are supposed to
				// *not* coalesce error characters into one string; therefore,
				// if we see an "empty" string here, we can safely assume the
				// lexer is reporting an occurrance of an illegal NUL in the
				// input stream
				if (val.error_msg[0] == 0) {
					out << " \"\\000\"";
				}
				else {
					out << " \"";
					print_escaped_string(out, val.error_msg);
					out << "\"";
					break;
				}
		}
		out << endl;
	}
	
}
