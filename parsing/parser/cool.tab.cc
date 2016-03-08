// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "cool.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "cool.tab.hh"

// User implementation prologue.

#line 51 "cool.tab.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 31 "cool.yy" // lalr1.cc:408

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
	

#line 92 "cool.tab.cc" // lalr1.cc:408


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 9 "cool.yy" // lalr1.cc:474
namespace parsing {
#line 178 "cool.tab.cc" // lalr1.cc:474

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
              // Fall through.
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
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
     : type (empty)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
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
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

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
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, scanner));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 143 "cool.yy" // lalr1.cc:847
    { 
			yylhs.location = yystack_[0].location; 
			driver.results()->set_root(program((yystack_[0].value.classes)));
	}
#line 631 "cool.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 150 "cool.yy" // lalr1.cc:847
    { (yylhs.value.classes) = single_Classes((yystack_[0].value.class_));
	    driver.results()->set_classes((yylhs.value.classes)); }
#line 638 "cool.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 153 "cool.yy" // lalr1.cc:847
    { (yylhs.value.classes) = append_Classes((yystack_[1].value.classes),single_Classes((yystack_[0].value.class_))); 
	    driver.results()->set_classes((yylhs.value.classes)); }
#line 645 "cool.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 160 "cool.yy" // lalr1.cc:847
    { 
				(yylhs.value.class_) = class_((yystack_[4].value.symbol),GlobalTables::getInstance().get_constants().Object,(yystack_[2].value.features), \
				GlobalTables::getInstance().stringtable.add_string(driver.get_filename())); 
				classFeaturesTracker::getInstance().applyFeaturesToClassAndReset((yylhs.value.class_));
			}
#line 655 "cool.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 166 "cool.yy" // lalr1.cc:847
    { 
				(yylhs.value.class_) = class_((yystack_[6].value.symbol),(yystack_[4].value.symbol),(yystack_[2].value.features),GlobalTables::getInstance().stringtable.add_string(driver.get_filename())); 
				classFeaturesTracker::getInstance().applyFeaturesToClassAndReset((yylhs.value.class_));
			}
#line 664 "cool.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 176 "cool.yy" // lalr1.cc:847
    {  (yylhs.value.features) = nil_Features(); }
#line 670 "cool.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 178 "cool.yy" // lalr1.cc:847
    { (yylhs.value.features) = append_Features(single_Features((yystack_[1].value.feature)),(yystack_[0].value.features)); }
#line 676 "cool.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 184 "cool.yy" // lalr1.cc:847
    { (yylhs.value.feature) = method((yystack_[9].value.symbol),(yystack_[7].value.formals),(yystack_[4].value.symbol),(yystack_[2].value.expression));
				classFeaturesTracker::getInstance().addFeature((yylhs.value.feature));
			}
#line 684 "cool.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 188 "cool.yy" // lalr1.cc:847
    { 
				(yylhs.value.feature) = attr((yystack_[5].value.symbol), (yystack_[3].value.symbol), (yystack_[1].value.expression));
				classFeaturesTracker::getInstance().addFeature((yylhs.value.feature));
			}
#line 693 "cool.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 193 "cool.yy" // lalr1.cc:847
    { 
				(yylhs.value.feature) = attr((yystack_[3].value.symbol), (yystack_[1].value.symbol), no_expr()); 
				classFeaturesTracker::getInstance().addFeature((yylhs.value.feature));
			}
#line 702 "cool.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 201 "cool.yy" // lalr1.cc:847
    { (yylhs.value.formals) = nil_Formals();  }
#line 708 "cool.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 203 "cool.yy" // lalr1.cc:847
    { (yylhs.value.formals) = append_Formals(single_Formals((yystack_[1].value.formal)),(yystack_[0].value.formals)); }
#line 714 "cool.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 208 "cool.yy" // lalr1.cc:847
    {(yylhs.value.formals) = nil_Formals(); }
#line 720 "cool.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 210 "cool.yy" // lalr1.cc:847
    { (yylhs.value.formals) = append_Formals(single_Formals((yystack_[1].value.formal)), (yystack_[0].value.formals));   }
#line 726 "cool.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 215 "cool.yy" // lalr1.cc:847
    { (yylhs.value.formal) = formal((yystack_[2].value.symbol), (yystack_[0].value.symbol));  }
#line 732 "cool.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 220 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = assign((yystack_[2].value.symbol),(yystack_[0].value.expression)); }
#line 738 "cool.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 222 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = (yystack_[0].value.expression); }
#line 744 "cool.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 224 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = cond((yystack_[5].value.expression),(yystack_[3].value.expression),(yystack_[1].value.expression)); }
#line 750 "cool.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 226 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = loop((yystack_[3].value.expression),(yystack_[1].value.expression)); }
#line 756 "cool.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 228 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = block((yystack_[0].value.expressions)); }
#line 762 "cool.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 230 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = let((yystack_[3].value.symbol),(yystack_[1].value.symbol),no_expr(),(yystack_[0].value.expression)); }
#line 768 "cool.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 232 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = let((yystack_[5].value.symbol),(yystack_[3].value.symbol),(yystack_[1].value.expression),(yystack_[0].value.expression)); }
#line 774 "cool.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 234 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = typcase((yystack_[4].value.expression),append_Cases(single_Cases((yystack_[2].value.case_)),(yystack_[1].value.cases))); }
#line 780 "cool.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 236 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = new_((yystack_[0].value.symbol)); }
#line 786 "cool.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 238 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = isvoid((yystack_[0].value.expression)); }
#line 792 "cool.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 240 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = (yystack_[0].value.expression); }
#line 798 "cool.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 242 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = object((yystack_[0].value.symbol)); }
#line 804 "cool.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 244 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = int_const((yystack_[0].value.symbol)); }
#line 810 "cool.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 246 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = string_const((yystack_[0].value.symbol)); }
#line 816 "cool.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 248 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = bool_const((yystack_[0].value.boolean)); }
#line 822 "cool.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 253 "cool.yy" // lalr1.cc:847
    {(yylhs.value.expression) = (yystack_[0].value.expression); }
#line 828 "cool.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 255 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = let((yystack_[5].value.symbol),(yystack_[3].value.symbol),(yystack_[1].value.expression),(yystack_[0].value.expression)); }
#line 834 "cool.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 257 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = let((yystack_[3].value.symbol),(yystack_[1].value.symbol),no_expr(),(yystack_[0].value.expression)); }
#line 840 "cool.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 270 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = (yystack_[0].value.expression); }
#line 846 "cool.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 275 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = static_dispatch((yystack_[7].value.expression), (yystack_[5].value.symbol), (yystack_[3].value.symbol), (yystack_[1].value.expressions));  }
#line 852 "cool.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 277 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = dispatch((yystack_[5].value.expression),(yystack_[3].value.symbol),(yystack_[1].value.expressions)); }
#line 858 "cool.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 279 "cool.yy" // lalr1.cc:847
    {
			IdEntryP entrada = GlobalTables::getInstance().idtable.add_string("self");
			(yylhs.value.expression) = dispatch(object(entrada),(yystack_[3].value.symbol),(yystack_[1].value.expressions));
			}
#line 867 "cool.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 287 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = nil_Expressions(); }
#line 873 "cool.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 289 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = append_Expressions(single_Expressions((yystack_[1].value.expression)),(yystack_[0].value.expressions)); }
#line 879 "cool.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 294 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = nil_Expressions(); }
#line 885 "cool.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 296 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = append_Expressions(single_Expressions((yystack_[1].value.expression)),(yystack_[0].value.expressions)); }
#line 891 "cool.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 301 "cool.yy" // lalr1.cc:847
    {(yylhs.value.case_) = branch((yystack_[5].value.symbol),(yystack_[3].value.symbol),(yystack_[1].value.expression)); }
#line 897 "cool.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 306 "cool.yy" // lalr1.cc:847
    {(yylhs.value.cases) = nil_Cases(); }
#line 903 "cool.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 308 "cool.yy" // lalr1.cc:847
    {(yylhs.value.cases) = append_Cases(single_Cases((yystack_[1].value.case_)),(yystack_[0].value.cases));}
#line 909 "cool.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 313 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = append_Expressions(single_Expressions((yystack_[3].value.expression)),(yystack_[1].value.expressions)); }
#line 915 "cool.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 319 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = nil_Expressions(); }
#line 921 "cool.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 321 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expressions) = append_Expressions(single_Expressions((yystack_[2].value.expression)),(yystack_[0].value.expressions)); }
#line 927 "cool.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 327 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = plus((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 933 "cool.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 329 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = sub((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 939 "cool.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 331 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = mul((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 945 "cool.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 333 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = divide((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 951 "cool.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 335 "cool.yy" // lalr1.cc:847
    {(yylhs.value.expression) = neg((yystack_[0].value.expression)); }
#line 957 "cool.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 337 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = lt((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 963 "cool.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 339 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = leq((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 969 "cool.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 341 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = eq((yystack_[2].value.expression),(yystack_[0].value.expression)); }
#line 975 "cool.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 343 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = comp((yystack_[0].value.expression)); }
#line 981 "cool.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 345 "cool.yy" // lalr1.cc:847
    { (yylhs.value.expression) = (yystack_[1].value.expression); }
#line 987 "cool.tab.cc" // lalr1.cc:847
    break;


#line 991 "cool.tab.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
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
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
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

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
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
         yyla.  (However, yyla is currently not documented for users.)
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
            // Stay within bounds of both yycheck and yytname.
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

    char const* yyformat = YY_NULLPTR;
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


  const signed char Parser::yypact_ninf_ = -117;

  const signed char Parser::yytable_ninf_ = -56;

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

  const short int
  Parser::yypgoto_[] =
  {
    -117,  -117,  -117,   169,    -3,  -117,  -117,   135,   145,   -35,
     -97,    18,    67,  -117,  -116,    55,   103,    61,  -117,   -70,
    -117
  };

  const short int
  Parser::yydefgoto_[] =
  {
      -1,     3,     4,     5,    15,    16,    26,    34,    27,   114,
     125,   156,   126,    56,    91,   111,   129,   130,    57,   115,
      58
  };

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



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
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
  "expr_block", "expr_list", "arith_logical", YY_NULLPTR
  };

#if YYDEBUG
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
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
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
    const unsigned int user_token_number_max_ = 284;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 9 "cool.yy" // lalr1.cc:1155
} // parsing
#line 1560 "cool.tab.cc" // lalr1.cc:1155
#line 349 "cool.yy" // lalr1.cc:1156


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
    
