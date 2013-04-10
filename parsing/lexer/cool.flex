/*
 *  The scanner definition for COOL.
 */

%{
//#include <cool-parse.h>
#include <stringtab.h>
#include <basic.h>
#include "LexerWrapper.h"
#include "ParserDriver.h"

using parsing::Parser;
using parsing::TokenHelper;

//for character-conversion issues
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[parsing::Lexer::max_str_length]; /* to assemble string constants */
char *string_buf_ptr;

int str_buf_len = 0;

int current_comment = 0;

%}

%option c++
%option noyywrap
%option yylineno
%option yyclass="parsing::Lexer"

/*
* Start conditions
*/

%x MULTILINE_COMMENT

%x SKIP_COMMENT

%x STRING

%x SKIP_STRING

/*
 * Define names for regular expressions here.
 */

/*
* Cool standard Syntax & single-line comment
*/

SINGLE_LINE_COMMENT \-\-.*[\n<<EOF>>] 

DIGIT [0-9]

KEYWORD (?i:class|else|fi|if|in|inherits|isvoid|let|loop|pool|then|while|case|esac|new|of|not)

TYPE_IDENTIFIER [A-Z][a-zA-Z0-9_]*

OBJECT_IDENTIFIER [a-z][a-zA-Z0-9_]*

SINGLE_CHAR_SPECIAL_NOTATION \{|\}|\(|\)|\;|\:|\+|\-|\*|\/|\~|\<|\=|\@|\.|\,

LESS_OR_EQUAL   <=

DARROW          =>

ASSIGN	<-

%%

 /*
  *  Nested comments
  */

<SKIP_COMMENT>{

[^*\n]*

<<EOF>> {
	yylval->error_msg = "EOF in comment";
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

\n 	

\*\)	{
	yylval->error_msg = "(* not matched";
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

\*

}

"(*" {
	current_comment = 1;
	BEGIN(MULTILINE_COMMENT);
}

<MULTILINE_COMMENT>{

[^\\\(\*\n]* 

\*+[^\\\)\n]

\*+\n	

\n 	

<<EOF>> {
		yylval->error_msg = "EOF in comment";
		BEGIN(INITIAL);
		return Parser::token::ERROR;
	}

\\.

\(\*	{
	current_comment++;
}

[\(\*]

\*\)	{
	current_comment--;
	if (current_comment != 0) {
		BEGIN(SKIP_COMMENT);
	} else {
		BEGIN(INITIAL);
	}
}

}

\"	{ 
	string_buf_ptr = string_buf;
	str_buf_len = 0;
	BEGIN(STRING);
}

<SKIP_STRING>{

[^\"\\\n]*

\\[\t]*\n 

\n {
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

\" {
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

<<EOF>> {
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

}

<STRING>{

[^\\\"\n\0]* {
	char *yptr = yytext;
	int str_overflow = 0;

	while (*yptr && !str_overflow ) {
		str_overflow = (try_add_single_char_str(*yptr) == Parser::token::ERROR);
		yptr++;
	}
	
	if (str_overflow){
		BEGIN(SKIP_STRING);
	}

}

\\b {
	if (try_add_single_char_str('\b') == Parser::token::ERROR) {
		BEGIN(SKIP_STRING); 
	}
}

\\t {
	if (try_add_single_char_str('\t') == Parser::token::ERROR) {
		BEGIN(SKIP_STRING);
	}
}

\\f {
	if (try_add_single_char_str('\f') == Parser::token::ERROR) {
		BEGIN(SKIP_STRING);
	}
}

\\n {
	if (try_add_single_char_str('\n') == Parser::token::ERROR) {
		BEGIN(SKIP_STRING);
	}
}

\\[ \t]*\n	{ 
	if (try_add_single_char_str('\n') == Parser::token::ERROR) {
		BEGIN(SKIP_STRING);
	}
}

\\. {
	if (try_add_single_char_str(yytext[1]) == Parser::token::ERROR) {
		BEGIN(SKIP_STRING);
	}
}

\n	{ 
	yylval->error_msg = "Unterminated string constant";
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

\0 {
	yylval->error_msg = "String contains null character";
	BEGIN(SKIP_STRING);
}

<<EOF>>	{
	yylval->error_msg = "EOF in string constant";
	BEGIN(INITIAL);
	return Parser::token::ERROR;
}

 /* Nota: no hi ha res que digui que aixó és vàlid en COOL
\"\" {
	if (try_add_single_char_str('"') == Parser::token::ERROR) {
		BEGIN(INITIAL); 
		return Parser::token::ERROR;
	}
}
*/

\"	{
	*string_buf_ptr++ = '\0';
	yylval->symbol = GlobalTables::getInstance().stringtable.add_string(string_buf);
	BEGIN(INITIAL);
	return Parser::token::STR_CONST;
}

}

 /*
  *  The multiple-character operators.
  */

{SINGLE_LINE_COMMENT} 

{DARROW}		{ return (Parser::token::DARROW); }

{ASSIGN}	{return Parser::token::ASSIGN; }

\*\) {
	yylval->error_msg = "Unmatched *)";
	return Parser::token::ERROR;
}

{LESS_OR_EQUAL} {
	return Parser::token::LE;
}

{SINGLE_CHAR_SPECIAL_NOTATION} {
	return (int)yytext[0];
}

\n 

[ \t\f\r\v]+  

{DIGIT}+ {
	yylval->symbol = GlobalTables::getInstance().inttable.add_string(yytext);
	return Parser::token::INT_CONST;
}

{KEYWORD} {
	return TokenHelper::get_keyword_token(yytext);
}

t(?i:rue) {
	yylval->boolean = 1;
	return Parser::token::BOOL_CONST;
}

f(?i:alse) {
	yylval->boolean = 0;
	return Parser::token::BOOL_CONST;
}

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */

{TYPE_IDENTIFIER} {
	yylval->symbol = GlobalTables::getInstance().idtable.add_string(yytext);
	return Parser::token::TYPEID;
}

{OBJECT_IDENTIFIER} {
	yylval->symbol = GlobalTables::getInstance().idtable.add_string(yytext);
	return Parser::token::OBJECTID;
}

. {
	yylval->error_msg = yytext;
	return Parser::token::ERROR;
}

%%

