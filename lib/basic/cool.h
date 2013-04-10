//
// See stanford-copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "stanford-copyright.h"

#ifndef _COOL_H_
#define _COOL_H_

#include "cool-io.h"
#include "utilities.h"

class Entry;
typedef Entry *Symbol;

inline bool copy_Boolean(bool b) {return b; }
inline void assert_Boolean(bool) {}
inline void dump_Boolean(ostream& stream, int padding, bool b)
{ stream << pad(padding) << b << "\n"; }

Symbol copy_Symbol(Symbol);
void assert_Symbol(Symbol);
void dump_Symbol(ostream &,int,Symbol);

#endif
