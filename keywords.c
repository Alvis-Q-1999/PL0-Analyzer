#include <string.h>

#include "keywords.h"

const char *keywords[13] = { "begin", "call",  "const",	    "do",   "end",
			     "if",    "odd",   "procedure", "read", "then",
			     "var",   "while", "write" };

int
sym2key(const char *id)
{
	for (const char **ptr = keywords; ptr - keywords < 13; ptr++) {
		if (!strcmp(*ptr, id))
			return (ptr - keywords);
	}
	return -1;
}

SYMBOL
num2sym(int num)
{
	switch (num) {
	case 0:
		return beginsym;
	case 1:
		return callsym;
	case 2:
		return constsym;
	case 3:
		return dosym;
	case 4:
		return endsym;
	case 5:
		return ifsym;
	case 6:
		return oddsym;
	case 7:
		return proceduresym;
	case 8:
		return readsym;
	case 9:
		return thensym;
	case 10:
		return varsym;
	case 11:
		return whilesym;
	case 12:
		return writesym;
	default:
		return nul;
	}
}
