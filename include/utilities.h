// -*-Mode: C++;-*-
//
// See stanford-copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "stanford-copyright.h"

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <ctype.h>       // for isprint
#include <assert.h>
#include <cstdlib>

#include "cool-io.h"     // for cerr, <<, manipulators
#include "symbol.h"   // Symbol <-> String conversions

extern void fatal_error(char *);
extern void print_escaped_string(ostream& str, const char *s);
extern char * pad(int);
extern char * changeCase(char * a); 

#endif
