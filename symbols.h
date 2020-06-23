#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdio.h>

/* Enumulate of all the symbols */
typedef enum SYMBOL {
	nul,
	eof,
	plus,
	minus,
	times,
	slash,
	lparen,
	rparen,
	comma,
	semicolon,
	period,
	becomes,
	eql,
	neq,
	lss,
	gtr,
	leq,
	geq,
	number,
	ident,
	beginsym,
	callsym,
	constsym,
	dosym,
	endsym,
	ifsym,
	oddsym,
	proceduresym,
	readsym,
	thensym,
	varsym,
	whilesym,
	writesym
} SYMBOL;

/* Record position of the file */
typedef struct {
	int row;
	int col;
} pos_t;

/* Chain node of symbols */
typedef struct {
	unsigned no;
	char *value;
	int type;
	void *next;
} symbol_t;

#define PREALLOC_SYM_NUM 0x010

/* Wrapper for fgetc/ungetc */
int get_char();
int unget_char(int ch);

/* Get symbol from input file */
SYMBOL getsym();

/* Initialize variables */
void symbol_init();
/* Add a symbol to end of chain */
void symbol_add(int ch);
/*  */
void symbol_dump(symbol_t *symbol);

#endif /* SYMBOLS_H */
