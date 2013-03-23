//
// See stanford-copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "stanford-copyright.h"

#ifndef _COOL_H_
#define _COOL_H_

#include "cool-io.h"
#include "utilities.h"

/* a type renaming */
typedef int Boolean;
class Entry;
typedef Entry *Symbol;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
{ stream << pad(padding) << (int) b << "\n"; }

Symbol copy_Symbol(Symbol);
void assert_Symbol(Symbol);
void dump_Symbol(ostream &,int,Symbol);

#endif
