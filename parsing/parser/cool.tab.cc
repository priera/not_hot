/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */


/* Line 293 of lalr1.cc  */
#line 39 "cool.tab.cc"


#include "cool.tab.hh"

/* User implementation prologue.  */


/* Line 299 of lalr1.cc  */
#line 48 "cool.tab.cc"
/* Unqualified %code blocks.  */

/* Line 300 of lalr1.cc  */
#line 31 "cool.yy"

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
	



/* Line 300 of lalr1.cc  */
#line 94 "cool.tab.cc"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 382 of lalr1.cc  */
#line 9 "cool.yy"
namespace parsing {

/* Line 382 of lalr1.cc  */
#line 182 "cool.tab.cc"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (parsing::ParserDriver &driver_yyarg, parsing::Lexer &scanner_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg),
      scanner (scanner_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, scanner);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 690 of lalr1.cc  */
#line 143 "cool.yy"
    { 
			(yyloc) = (yylocation_stack_[(1) - (1)]); 
			driver.results()->set_root(program((yysemantic_stack_[(1) - (1)].classes)));
	}
    break;

  case 3:

/* Line 690 of lalr1.cc  */
#line 150 "cool.yy"
    { (yyval.classes) = single_Classes((yysemantic_stack_[(1) - (1)].class_));
	    driver.results()->set_classes((yyval.classes)); }
    break;

  case 4:

/* Line 690 of lalr1.cc  */
#line 153 "cool.yy"
    { (yyval.classes) = append_Classes((yysemantic_stack_[(2) - (1)].classes),single_Classes((yysemantic_stack_[(2) - (2)].class_))); 
	    driver.results()->set_classes((yyval.classes)); }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 160 "cool.yy"
    { 
				(yyval.class_) = class_((yysemantic_stack_[(6) - (2)].symbol),GlobalTables::getInstance().get_constants().Object,(yysemantic_stack_[(6) - (4)].features), \
				GlobalTables::getInstance().stringtable.add_string(driver.get_filename())); 
				classFeaturesTracker::getInstance().applyFeaturesToClassAndReset((yyval.class_));
			}
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 166 "cool.yy"
    { 
				(yyval.class_) = class_((yysemantic_stack_[(8) - (2)].symbol),(yysemantic_stack_[(8) - (4)].symbol),(yysemantic_stack_[(8) - (6)].features),GlobalTables::getInstance().stringtable.add_string(driver.get_filename())); 
				classFeaturesTracker::getInstance().applyFeaturesToClassAndReset((yyval.class_));
			}
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 176 "cool.yy"
    {  (yyval.features) = nil_Features(); }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 178 "cool.yy"
    { (yyval.features) = append_Features(single_Features((yysemantic_stack_[(2) - (1)].feature)),(yysemantic_stack_[(2) - (2)].features)); }
    break;

  case 11:

/* Line 690 of lalr1.cc  */
#line 184 "cool.yy"
    { (yyval.feature) = method((yysemantic_stack_[(10) - (1)].symbol),(yysemantic_stack_[(10) - (3)].formals),(yysemantic_stack_[(10) - (6)].symbol),(yysemantic_stack_[(10) - (8)].expression));
				classFeaturesTracker::getInstance().addFeature((yyval.feature));
			}
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 188 "cool.yy"
    { 
				(yyval.feature) = attr((yysemantic_stack_[(6) - (1)].symbol), (yysemantic_stack_[(6) - (3)].symbol), (yysemantic_stack_[(6) - (5)].expression));
				classFeaturesTracker::getInstance().addFeature((yyval.feature));
			}
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 193 "cool.yy"
    { 
				(yyval.feature) = attr((yysemantic_stack_[(4) - (1)].symbol), (yysemantic_stack_[(4) - (3)].symbol), no_expr()); 
				classFeaturesTracker::getInstance().addFeature((yyval.feature));
			}
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 201 "cool.yy"
    { (yyval.formals) = nil_Formals();  }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 203 "cool.yy"
    { (yyval.formals) = append_Formals(single_Formals((yysemantic_stack_[(2) - (1)].formal)),(yysemantic_stack_[(2) - (2)].formals)); }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 208 "cool.yy"
    {(yyval.formals) = nil_Formals(); }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 210 "cool.yy"
    { (yyval.formals) = append_Formals(single_Formals((yysemantic_stack_[(3) - (2)].formal)), (yysemantic_stack_[(3) - (3)].formals));   }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 215 "cool.yy"
    { (yyval.formal) = formal((yysemantic_stack_[(3) - (1)].symbol), (yysemantic_stack_[(3) - (3)].symbol));  }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 220 "cool.yy"
    { (yyval.expression) = assign((yysemantic_stack_[(3) - (1)].symbol),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 222 "cool.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 224 "cool.yy"
    { (yyval.expression) = cond((yysemantic_stack_[(7) - (2)].expression),(yysemantic_stack_[(7) - (4)].expression),(yysemantic_stack_[(7) - (6)].expression)); }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 226 "cool.yy"
    { (yyval.expression) = loop((yysemantic_stack_[(5) - (2)].expression),(yysemantic_stack_[(5) - (4)].expression)); }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 228 "cool.yy"
    { (yyval.expression) = block((yysemantic_stack_[(1) - (1)].expressions)); }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 230 "cool.yy"
    { (yyval.expression) = let((yysemantic_stack_[(5) - (2)].symbol),(yysemantic_stack_[(5) - (4)].symbol),no_expr(),(yysemantic_stack_[(5) - (5)].expression)); }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 232 "cool.yy"
    { (yyval.expression) = let((yysemantic_stack_[(7) - (2)].symbol),(yysemantic_stack_[(7) - (4)].symbol),(yysemantic_stack_[(7) - (6)].expression),(yysemantic_stack_[(7) - (7)].expression)); }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 234 "cool.yy"
    { (yyval.expression) = typcase((yysemantic_stack_[(6) - (2)].expression),append_Cases(single_Cases((yysemantic_stack_[(6) - (4)].case_)),(yysemantic_stack_[(6) - (5)].cases))); }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 236 "cool.yy"
    { (yyval.expression) = new_((yysemantic_stack_[(2) - (2)].symbol)); }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 238 "cool.yy"
    { (yyval.expression) = isvoid((yysemantic_stack_[(2) - (2)].expression)); }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 240 "cool.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 30:

/* Line 690 of lalr1.cc  */
#line 242 "cool.yy"
    { (yyval.expression) = object((yysemantic_stack_[(1) - (1)].symbol)); }
    break;

  case 31:

/* Line 690 of lalr1.cc  */
#line 244 "cool.yy"
    { (yyval.expression) = int_const((yysemantic_stack_[(1) - (1)].symbol)); }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
#line 246 "cool.yy"
    { (yyval.expression) = string_const((yysemantic_stack_[(1) - (1)].symbol)); }
    break;

  case 33:

/* Line 690 of lalr1.cc  */
#line 248 "cool.yy"
    { (yyval.expression) = bool_const((yysemantic_stack_[(1) - (1)].boolean)); }
    break;

  case 34:

/* Line 690 of lalr1.cc  */
#line 253 "cool.yy"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 255 "cool.yy"
    { (yyval.expression) = let((yysemantic_stack_[(7) - (2)].symbol),(yysemantic_stack_[(7) - (4)].symbol),(yysemantic_stack_[(7) - (6)].expression),(yysemantic_stack_[(7) - (7)].expression)); }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 257 "cool.yy"
    { (yyval.expression) = let((yysemantic_stack_[(5) - (2)].symbol),(yysemantic_stack_[(5) - (4)].symbol),no_expr(),(yysemantic_stack_[(5) - (5)].expression)); }
    break;

  case 42:

/* Line 690 of lalr1.cc  */
#line 270 "cool.yy"
    { (yyval.expression) = (yysemantic_stack_[(2) - (2)].expression); }
    break;

  case 43:

/* Line 690 of lalr1.cc  */
#line 275 "cool.yy"
    { (yyval.expression) = static_dispatch((yysemantic_stack_[(8) - (1)].expression), (yysemantic_stack_[(8) - (3)].symbol), (yysemantic_stack_[(8) - (5)].symbol), (yysemantic_stack_[(8) - (7)].expressions));  }
    break;

  case 44:

/* Line 690 of lalr1.cc  */
#line 277 "cool.yy"
    { (yyval.expression) = dispatch((yysemantic_stack_[(6) - (1)].expression),(yysemantic_stack_[(6) - (3)].symbol),(yysemantic_stack_[(6) - (5)].expressions)); }
    break;

  case 45:

/* Line 690 of lalr1.cc  */
#line 279 "cool.yy"
    {
			IdEntryP entrada = GlobalTables::getInstance().idtable.add_string("self");
			(yyval.expression) = dispatch(object(entrada),(yysemantic_stack_[(4) - (1)].symbol),(yysemantic_stack_[(4) - (3)].expressions));
			}
    break;

  case 46:

/* Line 690 of lalr1.cc  */
#line 287 "cool.yy"
    { (yyval.expressions) = nil_Expressions(); }
    break;

  case 47:

/* Line 690 of lalr1.cc  */
#line 289 "cool.yy"
    { (yyval.expressions) = append_Expressions(single_Expressions((yysemantic_stack_[(2) - (1)].expression)),(yysemantic_stack_[(2) - (2)].expressions)); }
    break;

  case 48:

/* Line 690 of lalr1.cc  */
#line 294 "cool.yy"
    { (yyval.expressions) = nil_Expressions(); }
    break;

  case 49:

/* Line 690 of lalr1.cc  */
#line 296 "cool.yy"
    { (yyval.expressions) = append_Expressions(single_Expressions((yysemantic_stack_[(3) - (2)].expression)),(yysemantic_stack_[(3) - (3)].expressions)); }
    break;

  case 50:

/* Line 690 of lalr1.cc  */
#line 301 "cool.yy"
    {(yyval.case_) = branch((yysemantic_stack_[(6) - (1)].symbol),(yysemantic_stack_[(6) - (3)].symbol),(yysemantic_stack_[(6) - (5)].expression)); }
    break;

  case 51:

/* Line 690 of lalr1.cc  */
#line 306 "cool.yy"
    {(yyval.cases) = nil_Cases(); }
    break;

  case 52:

/* Line 690 of lalr1.cc  */
#line 308 "cool.yy"
    {(yyval.cases) = append_Cases(single_Cases((yysemantic_stack_[(2) - (1)].case_)),(yysemantic_stack_[(2) - (2)].cases));}
    break;

  case 53:

/* Line 690 of lalr1.cc  */
#line 313 "cool.yy"
    { (yyval.expressions) = append_Expressions(single_Expressions((yysemantic_stack_[(5) - (2)].expression)),(yysemantic_stack_[(5) - (4)].expressions)); }
    break;

  case 55:

/* Line 690 of lalr1.cc  */
#line 319 "cool.yy"
    { (yyval.expressions) = nil_Expressions(); }
    break;

  case 56:

/* Line 690 of lalr1.cc  */
#line 321 "cool.yy"
    { (yyval.expressions) = append_Expressions(single_Expressions((yysemantic_stack_[(3) - (1)].expression)),(yysemantic_stack_[(3) - (3)].expressions)); }
    break;

  case 58:

/* Line 690 of lalr1.cc  */
#line 327 "cool.yy"
    { (yyval.expression) = plus((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 59:

/* Line 690 of lalr1.cc  */
#line 329 "cool.yy"
    { (yyval.expression) = sub((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 60:

/* Line 690 of lalr1.cc  */
#line 331 "cool.yy"
    { (yyval.expression) = mul((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 61:

/* Line 690 of lalr1.cc  */
#line 333 "cool.yy"
    { (yyval.expression) = divide((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 62:

/* Line 690 of lalr1.cc  */
#line 335 "cool.yy"
    {(yyval.expression) = neg((yysemantic_stack_[(2) - (2)].expression)); }
    break;

  case 63:

/* Line 690 of lalr1.cc  */
#line 337 "cool.yy"
    { (yyval.expression) = lt((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 64:

/* Line 690 of lalr1.cc  */
#line 339 "cool.yy"
    { (yyval.expression) = leq((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 65:

/* Line 690 of lalr1.cc  */
#line 341 "cool.yy"
    { (yyval.expression) = eq((yysemantic_stack_[(3) - (1)].expression),(yysemantic_stack_[(3) - (3)].expression)); }
    break;

  case 66:

/* Line 690 of lalr1.cc  */
#line 343 "cool.yy"
    { (yyval.expression) = comp((yysemantic_stack_[(2) - (2)].expression)); }
    break;

  case 67:

/* Line 690 of lalr1.cc  */
#line 345 "cool.yy"
    { (yyval.expression) = (yysemantic_stack_[(3) - (2)].expression); }
    break;



/* Line 690 of lalr1.cc  */
#line 906 "cool.tab.cc"
	default:
          break;
      }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate, int yytoken)
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = 0;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -117;
  const short int
  Parser::yypact_[] =
  {
        19,   -11,    11,    69,    90,  -117,  -117,    -1,  -117,  -117,
      41,    35,    42,    54,    43,    56,    35,    35,    35,    72,
      75,    59,  -117,    62,  -117,    60,    65,    58,   -13,  -117,
      66,    95,    71,    72,  -117,   163,  -117,  -117,  -117,    96,
      58,   163,    97,   163,   163,    99,   163,  -117,  -117,  -117,
     -20,   163,   163,   133,   163,   280,  -117,  -117,  -117,    82,
    -117,   227,    80,   201,   239,  -117,    36,   163,   163,   339,
      36,    85,   292,   266,   163,   163,   163,   163,   163,   163,
     163,   104,   107,  -117,   163,   163,   105,   163,   111,   339,
     250,    98,    91,    91,  -117,   350,   350,   350,    45,    45,
      36,    36,   100,   102,   328,   176,     3,   214,   101,   111,
     163,  -117,  -117,   108,   304,   106,   110,   114,   163,   116,
     163,    -6,   163,   163,   117,  -117,  -117,  -117,   127,   111,
     143,   250,    91,    91,  -117,  -117,   119,   120,  -117,   187,
       2,  -117,   339,    24,   118,   146,  -117,  -117,  -117,  -117,
    -117,   163,  -117,  -117,   121,   123,  -117,  -117,   141,   163,
     125,     2,   147,    23,   316,  -117,  -117,    64,   163,  -117,
    -117,   163,  -117,    24,    24,  -117,  -117
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     3,     7,     0,     1,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     9,     0,    10,     0,     0,    16,     0,     5,
       0,     0,     0,     0,    15,     0,    13,     6,    18,     0,
      16,     0,     0,     0,     0,     0,     0,    32,    31,    33,
      30,     0,     0,     0,     0,     0,    20,    23,    29,     0,
      17,     0,     0,     0,     0,    27,    28,     0,    46,    66,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    12,     0,     0,     0,     0,     0,    19,
      48,     0,     0,     0,    67,    64,    63,    65,    58,    59,
      60,    61,     0,     0,     0,     0,     0,     0,     0,    51,
       0,    47,    45,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,    24,    34,    22,     0,    51,
       0,    48,     0,     0,    54,    53,     0,     0,    11,     0,
       0,    38,    42,     0,     0,     0,    52,    26,    49,    57,
      56,    46,    44,    21,     0,     0,    37,    25,     0,     0,
       0,     0,     0,     0,     0,    43,    41,     0,     0,    36,
      50,     0,    40,     0,     0,    35,    39
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -117,  -117,  -117,   169,    -3,  -117,  -117,   135,   145,   -35,
     -97,    18,    67,  -117,  -116,    55,   103,    61,  -117,   -70,
    -117
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     3,     4,     5,    15,    16,    26,    34,    27,   114,
     125,   156,   126,    56,    91,   111,   129,   130,    57,   115,
      58
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Parser::yytable_ninf_ = -56;
  const short int
  Parser::yytable_[] =
  {
        55,   122,   137,   154,   121,    67,    61,    10,    63,    64,
     122,    66,    35,    22,    23,    24,    69,    70,    72,    73,
       1,    68,     2,   116,   121,   121,   155,    36,   123,     6,
     122,   122,    89,    90,     7,   160,    13,    11,   140,    95,
      96,    97,    98,    99,   100,   101,   157,   124,   168,   104,
     105,    74,   107,    75,    76,    77,    78,    79,    80,    14,
      81,    82,   149,   150,    12,   121,   169,   124,   124,     8,
     172,   122,    81,    82,    -8,   131,   175,   176,    79,    80,
      17,    81,    82,    90,    19,   139,    20,   142,   143,   171,
      -2,     1,   113,     2,    18,    21,    25,    41,    28,    29,
      42,    30,    33,    31,    43,    44,    37,    32,   124,    45,
      46,    47,    48,    49,    39,    50,    90,    51,    38,    59,
      84,    62,    65,    86,   164,    92,    52,   102,   106,    53,
     -55,   103,    54,   173,    71,   108,   174,   117,   136,    41,
     112,   144,    42,   118,   128,   134,    43,    44,   132,   135,
     145,    45,    46,    47,    48,    49,   138,    50,   147,    51,
     151,   158,   152,   159,   163,   161,   162,   165,    52,    41,
     167,    53,    42,     9,    54,    60,    43,    44,    40,   166,
     120,    45,    46,    47,    48,    49,   148,    50,   141,    51,
     146,   109,   153,     0,     0,     0,     0,     0,    52,     0,
       0,    53,     0,    74,    54,    75,    76,    77,    78,    79,
      80,    87,    81,    82,    74,     0,    75,    76,    77,    78,
      79,    80,     0,    81,    82,   127,     0,     0,    74,     0,
      75,    76,    77,    78,    79,    80,     0,    81,    82,    85,
       0,    74,     0,    75,    76,    77,    78,    79,    80,     0,
      81,    82,     0,     0,    74,    88,    75,    76,    77,    78,
      79,    80,     0,    81,    82,     0,    74,     0,    75,    76,
      77,    78,    79,    80,     0,    81,    82,    74,     0,    75,
      76,    77,    78,    79,    80,     0,    81,    82,     0,     0,
       0,     0,     0,    74,   110,    75,    76,    77,    78,    79,
      80,     0,    81,    82,     0,     0,     0,    74,    94,    75,
      76,    77,    78,    79,    80,     0,    81,    82,     0,    74,
      83,    75,    76,    77,    78,    79,    80,     0,    81,    82,
       0,    74,    93,    75,    76,    77,    78,    79,    80,     0,
      81,    82,     0,    74,   133,    75,    76,    77,    78,    79,
      80,     0,    81,    82,     0,    74,   170,    75,    76,    77,
      78,    79,    80,     0,    81,    82,    74,   119,    75,    76,
      77,    78,    79,    80,     0,    81,    82,   -56,     0,   -56,
     -56,    77,    78,    79,    80,     0,    81,    82
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        35,     7,   118,     1,     1,    25,    41,     8,    43,    44,
       7,    46,    25,    16,    17,    18,    51,    52,    53,    54,
       1,    41,     3,    93,     1,     1,    24,    40,    25,    40,
       7,     7,    67,    68,    23,   151,     1,    38,    44,    74,
      75,    76,    77,    78,    79,    80,   143,    44,    25,    84,
      85,    27,    87,    29,    30,    31,    32,    33,    34,    24,
      36,    37,   132,   133,    23,     1,   163,    44,    44,     0,
     167,     7,    36,    37,    39,   110,   173,   174,    33,    34,
      38,    36,    37,   118,    41,   120,    43,   122,   123,    25,
       0,     1,     1,     3,    40,    39,    24,     6,    23,    40,
       9,    39,    44,    43,    13,    14,    40,    42,    44,    18,
      19,    20,    21,    22,    43,    24,   151,    26,    23,    23,
      38,    24,    23,    43,   159,    40,    35,    23,    23,    38,
      39,    24,    41,   168,     1,    24,   171,    37,    24,     6,
      42,    24,     9,    41,    43,    39,    13,    14,    40,    39,
      23,    18,    19,    20,    21,    22,    40,    24,    15,    26,
      41,    43,    42,    17,    23,    44,    43,    42,    35,     6,
      23,    38,     9,     4,    41,    40,    13,    14,    33,   161,
       4,    18,    19,    20,    21,    22,   131,    24,   121,    26,
     129,    88,     5,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    38,    -1,    27,    41,    29,    30,    31,    32,    33,
      34,    10,    36,    37,    27,    -1,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    11,    -1,    -1,    27,    -1,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    12,
      -1,    27,    -1,    29,    30,    31,    32,    33,    34,    -1,
      36,    37,    -1,    -1,    27,    16,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    -1,    27,    -1,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    27,    -1,    29,
      30,    31,    32,    33,    34,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    27,    44,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    -1,    -1,    -1,    27,    42,    29,
      30,    31,    32,    33,    34,    -1,    36,    37,    -1,    27,
      40,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      -1,    27,    40,    29,    30,    31,    32,    33,    34,    -1,
      36,    37,    -1,    27,    40,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    -1,    27,    40,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,    27,    39,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    27,    -1,    29,
      30,    31,    32,    33,    34,    -1,    36,    37
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     3,    46,    47,    48,    40,    23,     0,    48,
       8,    38,    23,     1,    24,    49,    50,    38,    40,    41,
      43,    39,    49,    49,    49,    24,    51,    53,    23,    40,
      39,    43,    42,    44,    52,    25,    40,    40,    23,    43,
      53,     6,     9,    13,    14,    18,    19,    20,    21,    22,
      24,    26,    35,    38,    41,    54,    58,    63,    65,    23,
      52,    54,    24,    54,    54,    23,    54,    25,    41,    54,
      54,     1,    54,    54,    27,    29,    30,    31,    32,    33,
      34,    36,    37,    40,    38,    12,    43,    10,    16,    54,
      54,    59,    40,    40,    42,    54,    54,    54,    54,    54,
      54,    54,    23,    24,    54,    54,    23,    54,    24,    61,
      44,    60,    42,     1,    54,    64,    64,    37,    41,    39,
       4,     1,     7,    25,    44,    55,    57,    11,    43,    61,
      62,    54,    40,    40,    39,    39,    24,    59,    40,    54,
      44,    57,    54,    54,    24,    23,    62,    15,    60,    64,
      64,    41,    42,     5,     1,    24,    56,    55,    43,    17,
      59,    44,    43,    23,    54,    42,    56,    23,    25,    55,
      40,    25,    55,    54,    54,    55,    55
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   284,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    60,
      61,    43,    45,    42,    47,   126,    64,    46,   123,   125,
      59,    40,    41,    58,    44
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    45,    46,    47,    47,    48,    48,    48,    49,    49,
      49,    50,    50,    50,    51,    51,    52,    52,    53,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    55,    55,    55,    55,    55,    56,
      56,    56,    57,    58,    58,    58,    59,    59,    60,    60,
      61,    62,    62,    63,    63,    64,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     2,     6,     8,     2,     0,     2,
       3,    10,     6,     4,     0,     2,     0,     3,     3,     3,
       1,     7,     5,     1,     5,     7,     6,     2,     2,     1,
       1,     1,     1,     1,     1,     7,     5,     3,     2,     6,
       4,     3,     2,     8,     6,     4,     0,     2,     0,     3,
       6,     0,     2,     5,     5,     0,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     2,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "$end", "error", "$undefined", "CLASS", "ELSE", "FI", "IF", "IN",
  "INHERITS", "LET", "LOOP", "POOL", "THEN", "WHILE", "CASE", "ESAC", "OF",
  "DARROW", "NEW", "ISVOID", "STR_CONST", "INT_CONST", "BOOL_CONST",
  "TYPEID", "OBJECTID", "ASSIGN", "NOT", "LE", "ERROR", "'<'", "'='",
  "'+'", "'-'", "'*'", "'/'", "'~'", "'@'", "'.'", "'{'", "'}'", "';'",
  "'('", "')'", "':'", "','", "$accept", "program", "class_list", "class",
  "feature_list", "feature", "opt_formal_list", "formal_list", "formal",
  "expr", "let_declarations", "error_let", "let_end", "dispatch",
  "whole_argument_list", "argument_list", "case_branch", "case_branches",
  "expr_block", "expr_list", "arith_logical", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        46,     0,    -1,    47,    -1,    48,    -1,    47,    48,    -1,
       3,    23,    38,    49,    39,    40,    -1,     3,    23,     8,
      23,    38,    49,    39,    40,    -1,     1,    40,    -1,    -1,
      50,    49,    -1,     1,    40,    49,    -1,    24,    41,    51,
      42,    43,    23,    38,    54,    39,    40,    -1,    24,    43,
      23,    25,    54,    40,    -1,    24,    43,    23,    40,    -1,
      -1,    53,    52,    -1,    -1,    44,    53,    52,    -1,    24,
      43,    23,    -1,    24,    25,    54,    -1,    58,    -1,     6,
      54,    12,    54,     4,    54,     5,    -1,    13,    54,    10,
      54,    11,    -1,    63,    -1,     9,    24,    43,    23,    55,
      -1,     9,    24,    43,    23,    25,    54,    55,    -1,    14,
      54,    16,    61,    62,    15,    -1,    18,    23,    -1,    19,
      54,    -1,    65,    -1,    24,    -1,    21,    -1,    20,    -1,
      22,    -1,    57,    -1,    44,    24,    43,    23,    25,    54,
      55,    -1,    44,    24,    43,    23,    55,    -1,     1,    44,
      56,    -1,     1,    57,    -1,    24,    43,    23,    25,    54,
      55,    -1,    24,    43,    23,    55,    -1,     1,    44,    56,
      -1,     7,    54,    -1,    54,    36,    23,    37,    24,    41,
      59,    42,    -1,    54,    37,    24,    41,    59,    42,    -1,
      24,    41,    59,    42,    -1,    -1,    54,    60,    -1,    -1,
      44,    54,    60,    -1,    24,    43,    23,    17,    54,    40,
      -1,    -1,    61,    62,    -1,    38,    54,    40,    64,    39,
      -1,    38,     1,    40,    64,    39,    -1,    -1,    54,    40,
      64,    -1,     1,    40,    64,    -1,    54,    31,    54,    -1,
      54,    32,    54,    -1,    54,    33,    54,    -1,    54,    34,
      54,    -1,    35,    54,    -1,    54,    29,    54,    -1,    54,
      27,    54,    -1,    54,    30,    54,    -1,    26,    54,    -1,
      41,    54,    42,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,    10,    17,    26,    29,    30,
      33,    37,    48,    55,    60,    61,    64,    65,    69,    73,
      77,    79,    87,    93,    95,   101,   109,   116,   119,   122,
     124,   126,   128,   130,   132,   134,   142,   148,   152,   155,
     162,   167,   171,   174,   183,   190,   195,   196,   199,   200,
     204,   211,   212,   215,   221,   227,   228,   232,   236,   240,
     244,   248,   252,   255,   259,   263,   267,   270
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   143,   143,   149,   152,   159,   165,   170,   176,   177,
     179,   183,   187,   192,   201,   202,   208,   209,   214,   219,
     221,   223,   225,   227,   229,   231,   233,   235,   237,   239,
     241,   243,   245,   247,   252,   254,   256,   258,   259,   263,
     264,   265,   269,   274,   276,   278,   287,   288,   294,   295,
     300,   306,   307,   312,   314,   319,   320,   322,   326,   328,
     330,   332,   334,   336,   338,   340,   342,   344
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,    33,    31,    44,    32,    37,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    40,
      29,    30,     2,     2,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,    39,    35,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     2
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 387;
  const int Parser::yynnts_ = 21;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 8;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 45;

  const unsigned int Parser::yyuser_token_number_max_ = 284;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1136 of lalr1.cc  */
#line 9 "cool.yy"
} // parsing

/* Line 1136 of lalr1.cc  */
#line 1578 "cool.tab.cc"


/* Line 1138 of lalr1.cc  */
#line 349 "cool.yy"


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
    

