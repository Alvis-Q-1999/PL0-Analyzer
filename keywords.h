#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "symbols.h"

/* Check if is keyword */
int sym2key(const char *id);
/* Transform from number to SYMBOL */
SYMBOL num2sym(int num);

#endif /* KEYWORDS_H */
