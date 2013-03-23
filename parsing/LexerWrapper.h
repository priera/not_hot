#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Override the interface for yylex since we namespaced it
#undef YY_DECL
#define YY_DECL int parsing::Lexer::yylex()

#ifndef LEXER_WRAPPER_H
#define LEXER_WRAPPER_H

#include "cool.tab.hh"

extern int str_buf_len;
extern char *string_buf_ptr;

namespace parsing {
	class Lexer : public yyFlexLexer {
		public:
			// save the pointer to yylval so we can change it, and invoke scanner
			int yylex(parsing::Parser::semantic_type * lval) { yylval = lval; return yylex(); }
			
			Parser::semantic_type * get_yylval() { return yylval; };
			
			/* Max size of string constants */
			static const int max_str_length = 1025;
		private:
		

			
			//Appends char c to the string buffer, checking that won't be a buffer overflow
			int try_add_single_char_str(char c){
				if (str_buf_len < max_str_length -1) {
					*string_buf_ptr++ = c;
					str_buf_len++;
					return parsing::Parser::token::ERROR - 1;
				} else {
					yylval->error_msg = "String constant too long";
					return parsing::Parser::token::ERROR;
				}
			};
			
			// Scanning function created by Flex; make this private to force usage
			// of the overloaded method so we can get a pointer to Bison's yylval
			int yylex();
			
			// point to yylval (provided by Bison in overloaded yylex)
			Parser::semantic_type * yylval;
	};
}

#endif