%require "2.4.1"

/*template for c++ parser*/
%skeleton "lalr1.cc"
%debug
%error-verbose
%defines
%locations
%define namespace "parsing"
%define parser_class_name "Parser"

/*Generated parser will recive the driver and the scanner as arguments. The scanner will be an argument to the lexer function */
%parse-param { parsing::ParserDriver &driver }
%parse-param { parsing::Lexer &scanner }
%lex-param   { parsing::Lexer &scanner }

%code requires {
  #include <iostream>
  #include <basic.h>
  #include <tree.h>
  #include <cool-tree.h>
  #include <stringtab.h>
  
  //Forward declaration of parsing classes
	namespace parsing {
		class Lexer;
		class ParserDriver;
	}
}

%code {
   // Prototype for the yylex function
	static int yylex(parsing::Parser::semantic_type * yylval, parsing::Parser::location_type *location, parsing::Lexer &scanner );
	#include "ParserDriver.h"

	//Redefining location macro. It's the same code as the default, but it also updates de line number that will be used to generate the tree node
#define YYLLOC_DEFAULT(Current, Rhs, N)										\
		do {																				\
			if (N) {																		\
					(Current).begin = YYRHSLOC (Rhs, 1).begin;				\
					(Current).end =  YYRHSLOC (Rhs, N).end;					\
			} else {																		\
				(Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end; \
			}																				\
				driver.update_curr_lineno((Current).begin.line);			\ 
		} while (false)
		
	class classFeaturesTracker {
	private:
		Features current_class_features;
		classFeaturesTracker() : current_class_features(nil_Features()){ }
	public:
		static classFeaturesTracker & getInstance(){
			static classFeaturesTracker M;
			return M;
		}
		void addFeature(Feature f){
			current_class_features = append_Features(single_Features(f), current_class_features);
		};
		void applyFeaturesToClassAndReset(Class_ c){
			for (int i = current_class_features->first(); current_class_features->more(i); i = current_class_features->next(i)){
				current_class_features->nth(i)->set_container_class(c);
			}
			current_class_features = nil_Features();
		}
	};
	
} 
    
    /* A union of all the types that can be the result of parsing actions. */
    %union {
      bool boolean;
      Symbol symbol;
      Program program;
      Class_ class_;
      Classes classes;
      Feature feature;
      Features features;
      Formal formal;
      Formals formals;
      Case case_;
      Cases cases;
      Expression expression;
      Expressions expressions;
      char const *error_msg;
    }
    
    /* 
    Declare the terminals; a few have types for associated lexemes.
    The token ERROR is never used in the parser; thus, it is a parse
    error when the lexer returns it.
    
    The integer following token declaration is the numeric constant used
    to represent that token internally.  Typically, Bison generates these
    on its own, but we give explicit numbers to prevent version parity
    problems (bison 1.25 and earlier start at 258, later versions -- at
    257)
    */
    %token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
    %token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
    %token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
    %token <symbol>  STR_CONST 275 INT_CONST 276 
    %token <boolean> BOOL_CONST 277
    %token <symbol>  TYPEID 278 OBJECTID 279 
    %token ASSIGN 280 NOT 281 LE 282 ERROR 283
    
    /* Declare types for the grammar's non-terminals. */
	%type <program> program
	%type <classes> class_list
	%type <class_> class
	%type <feature> feature
	%type <features> feature_list
	%type <formals> opt_formal_list
	%type <formals> formal_list
	%type <formal> formal
 	%type <expression> expr
	%type <expressions> expr_block
	%type <expressions> expr_list
	%type <expression> let_declarations
	%type <expression> let_end
	%type <expression> dispatch
	%type <expressions> whole_argument_list
	%type <expressions> argument_list
	%type <case_> case_branch
	%type <cases> case_branches
	%type <expression> arith_logical

    /* Precedence declarations */
	%right ASSIGN
	%left NOT
	%nonassoc LE '<' '=' 
	%left '+' '-'
	%left '*' '/'
	%left ISVOID
	%left '~'
	%left '@'
	%left '.'

    %%
    /* 
    Save the root of the abstract syntax tree in a global variable.
    */
    program	: class_list	{ 
			@$ = @1; 
			driver.results()->set_root(program($1));
	};
    
    class_list
	: class			/* single class */
	    { $$ = single_Classes($1);
	    driver.results()->set_classes($$); }
	| class_list class	/* several classes */
	    { $$ = append_Classes($1,single_Classes($2)); 
	    driver.results()->set_classes($$); }
    ;
    
    /* If no parent is specified, the class inherits from the Object class. */
    class	: 
	CLASS TYPEID '{' feature_list '}' ';'
			{ 
				$$ = class_($2,GlobalTables::getInstance().get_constants().Object,$4, \
				GlobalTables::getInstance().stringtable.add_string(driver.get_filename())); 
				classFeaturesTracker::getInstance().applyFeaturesToClassAndReset($$);
			}
    	| CLASS TYPEID INHERITS TYPEID '{' feature_list '}' ';'
			{ 
				$$ = class_($2,$4,$6,GlobalTables::getInstance().stringtable.add_string(driver.get_filename())); 
				classFeaturesTracker::getInstance().applyFeaturesToClassAndReset($$);
			}
	| error ';'
    ;
    
    /* Feature list may be empty, but no empty features in list. */
    feature_list:		
	/* empty */
	    	{  $$ = nil_Features(); }
   	| feature feature_list
		{ $$ = append_Features(single_Features($1),$2); }
	| error ';' feature_list 
    ;

	feature: 
		OBJECTID '(' opt_formal_list ')' ':' TYPEID '{' expr '}' ';' 
			{ $$ = method($1,$3,$6,$8);
				classFeaturesTracker::getInstance().addFeature($$);
			}
		| OBJECTID ':' TYPEID ASSIGN expr ';'
			{ 
				$$ = attr($1, $3, $5);
				classFeaturesTracker::getInstance().addFeature($$);
			}
		| OBJECTID ':' TYPEID ';'
			{ 
				$$ = attr($1, $3, no_expr()); 
				classFeaturesTracker::getInstance().addFeature($$);
			}
	;

	opt_formal_list:
		/*empty*/
			{ $$ = nil_Formals();  }
		| formal formal_list
			{ $$ = append_Formals(single_Formals($1),$2); }
	;
	
	formal_list :
		/* empty */
			{$$ = nil_Formals(); }
		| ',' formal formal_list
			{ $$ = append_Formals(single_Formals($2), $3);   }
	;
	
	formal : 
		OBJECTID ':' TYPEID
			{ $$ = formal($1, $3);  }
	;

	expr: 
		OBJECTID ASSIGN expr
			{ $$ = assign($1,$3); }
		| dispatch 
			{ $$ = $1; }
		| IF expr THEN expr ELSE expr FI
			{ $$ = cond($2,$4,$6); }
		| WHILE expr LOOP expr POOL
			{ $$ = loop($2,$4); }
		| expr_block 
			{ $$ = block($1); }
		| LET OBJECTID ':' TYPEID let_declarations
			{ $$ = let($2,$4,no_expr(),$5); }
		| LET OBJECTID ':' TYPEID ASSIGN expr let_declarations
			{ $$ = let($2,$4,$6,$7); }
		| CASE expr OF case_branch case_branches  ESAC
			{ $$ = typcase($2,append_Cases(single_Cases($4),$5)); }
		| NEW TYPEID
			{ $$ = new_($2); }
		| ISVOID expr
			{ $$ = isvoid($2); }
		| arith_logical
			{ $$ = $1; }
		| OBJECTID
			{ $$ = object($1); }
		| INT_CONST
			{ $$ = int_const($1); }
		| STR_CONST 
			{ $$ = string_const($1); }
		| BOOL_CONST
			{ $$ = bool_const($1); } 
	;
	
	let_declarations:
		let_end
			{$$ = $1; }
		| ',' OBJECTID ':' TYPEID ASSIGN expr let_declarations
			{ $$ = let($2,$4,$6,$7); }
		| ',' OBJECTID ':' TYPEID let_declarations
			{ $$ = let($2,$4,no_expr(),$5); }
		| error ',' error_let
		| error let_end
	; 

	error_let:
		OBJECTID ':' TYPEID ASSIGN expr let_declarations
		| OBJECTID ':' TYPEID let_declarations
		| error ',' error_let
	;
	
	let_end:
		IN expr
			{ $$ = $2; }
	;

	dispatch:
		expr '@' TYPEID '.' OBJECTID '(' whole_argument_list ')' 
			{ $$ = static_dispatch($1, $3, $5, $7);  } 
		| expr '.' OBJECTID '(' whole_argument_list ')' 
			{ $$ = dispatch($1,$3,$5); }
		| OBJECTID '(' whole_argument_list ')'
			{
			IdEntryP entrada = GlobalTables::getInstance().idtable.add_string("self");
			$$ = dispatch(object(entrada),$1,$3);
			}
	;

	whole_argument_list:
		/*empty*/
			{ $$ = nil_Expressions(); }
		| expr argument_list
			{ $$ = append_Expressions(single_Expressions($1),$2); }	
	;

	argument_list:
		/*empty*/
			{ $$ = nil_Expressions(); }
		 | ',' expr argument_list
			{ $$ = append_Expressions(single_Expressions($2),$3); }
	;

	case_branch: 
		OBJECTID ':' TYPEID DARROW expr ';'
			{$$ = branch($1,$3,$5); }
	;

	case_branches: 
		/*empty*/
			{$$ = nil_Cases(); }
		| case_branch case_branches
			{$$ = append_Cases(single_Cases($1),$2);} 
	;

	expr_block:
		'{' expr ';' expr_list '}'
			{ $$ = append_Expressions(single_Expressions($2),$4); }
		| '{' error ';' expr_list '}'
	;	

	expr_list: 
		/*empty*/
			{ $$ = nil_Expressions(); }
		| expr ';' expr_list
			{ $$ = append_Expressions(single_Expressions($1),$3); }
		| error ';' expr_list
	;

	arith_logical:
		expr '+' expr
			{ $$ = plus($1,$3); }
		| expr '-' expr
			{ $$ = sub($1,$3); }
		| expr '*' expr 
			{ $$ = mul($1,$3); }
		| expr '/' expr
			{ $$ = divide($1,$3); }
		| '~' expr
			{$$ = neg($2); }
		| expr '<' expr
			{ $$ = lt($1,$3); }
		| expr LE expr
			{ $$ = leq($1,$3); }
		| expr '=' expr
			{ $$ = eq($1,$3); }
		| NOT expr
			{ $$ = comp($2); }
		| '(' expr ')' 
			{ $$ = $2; }
	;

    /* end of grammar */
    %%

	/*Error reporting function*/
	void parsing::Parser::error(const parsing::Parser::location_type &loc, const std::string &msg) {
		cerr << "\"" << driver.get_filename() << "\", at " << loc << ": " << msg << endl;

      driver.results()->incErrorsCount();      
      driver.check_error_count();
	}
	
 // Now that we have the Parser declared, we can declare the Scanner and implement
// the yylex function
#include <LexerWrapper.h>
static int yylex(parsing::Parser::semantic_type * yylval, parsing::Parser::location_type *location, parsing::Lexer &scanner)  {
	int ret = scanner.yylex(yylval);
	/*parsing::TokenHelper helper((parsing::Parser::token_type) ret, *yylval);
	helper.dump(cout, scanner.lineno());*/
	
	location->begin = parsing::position();
	location->begin.lines(scanner.lineno() - 1);
	location->end = parsing::position();
	location->end.lines(scanner.lineno() - 1);
	return ret;
}
    
