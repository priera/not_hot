#include "lineno_mng.h"

int Lineno_Mngr::curr_lineno;

void Lineno_Mngr::reset() {
		curr_lineno = 1;
}

