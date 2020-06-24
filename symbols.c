#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "symbols.h"
#include "keywords.h"

/* human readable */
const char *symtype[33] = {
	"nul",	  "eof",      "plus",	  "minus",	  "times",    "slash",
	"lparen", "rparen",   "comma",	  "seicolon",	  "period",   "becomes",
	"eql",	  "neq",      "lss",	  "gtr",	  "leq",      "geq",
	"number", "ident",    "beginsym", "callsym",	  "consysym", "dosym",
	"endsym", "ifsym",    "oddsym",	  "proceduresym", "readsym",  "thensym",
	"varsym", "whilesym", "writesym"
};

/* Current position and error position */
pos_t cur, err;
/* Preallocated symbol chain */
token_t tokens[PREALLOC_SYM_NUM];
/* Head and tail of the chain, the chain itself is hidden to others */
token_t *token_tail = tokens;

/* Max length of an ident */
#define MAX_IDENT 20

/* Ident variable */
char id[MAX_IDENT] = "";
/* Ident length */
int id_len;

/* Count for symbols */
int token_num;

int
get_char()
{
	int ch = fgetc(stdin);
	cur.col++;
	if (ch == '\n') {
		cur.row++;
		cur.col = 0;
	}
	return ch;
}

int
unget_char(int ch)
{
	ungetc(ch, stdin);
	cur.col--;
	if (ch == '\n')
		cur.row--;
	return 0;
}

/* Save symbol to id and return */
#define SAVE_SYM(str, sym)                                                     \
	{                                                                      \
		strcpy(id, str);                                               \
		return sym;                                                    \
	}

/* Print error info if symbol cannot be recongnized */
#define invalid()                                                              \
	{                                                                      \
		fprintf(stderr, "lex:%d:%d: invalid symbol: %s\n", err.row,    \
			err.col, id);                                          \
		return nul;                                                    \
	}

SYMBOL
getsym()
{
	int ch;
	char invalid[2] = "\0";

	while ((ch = get_char()) != EOF && ch <= ' ')
		;

	err.row = cur.row;
	err.col = cur.col;
	token_num++;

	switch (ch) {
	case EOF:
		SAVE_SYM("EOF", eof)
	case '+':
		SAVE_SYM("+", plus)
	case '-':
		SAVE_SYM("-", minus)
	case '*':
		SAVE_SYM("*", times)
	case '/':
		SAVE_SYM("/", slash)
	case '(':
		SAVE_SYM("(", lparen)
	case ')':
		SAVE_SYM(")", rparen)
	case ',':
		SAVE_SYM(",", comma)
	case ';':
		SAVE_SYM(";", semicolon)
	case '.':
		SAVE_SYM(".", period)
	case ':':
		ch = get_char();
		if (ch == '=') {
			SAVE_SYM(":=", becomes)
		} else
			return nul;
	case '=':
		SAVE_SYM("=", eql)
	case '#':
		SAVE_SYM("#", neq)
	case '<':
		ch = get_char();
		if (ch == '=') {
			SAVE_SYM("<=", leq)
		}
		unget_char(ch);
		SAVE_SYM("<", lss)
	case '>':
		ch = get_char();
		if (ch == '=')
			SAVE_SYM(">=", geq)

		unget_char(ch);
		SAVE_SYM(">", gtr)
	default:
		if (isdigit(ch)) {
			int num = 0;
			do {
				num = 10 * num + ch - '0';
				ch = get_char();
			} while (ch != EOF && isdigit(ch));
			sprintf(id, "%d", num);
			if (isalpha(ch)) {
				invalid[0] = (char)ch;
				strcat(id, invalid);
				ch = get_char();
				while (isalnum(ch)) {
					invalid[0] = (char)ch;
					strcat(id, invalid);
					ch = get_char();
				}
				invalid();
			}
			unget_char(ch);
			return number;
		} else if (isalpha(ch)) {
			id_len = 0;
			do {
				if (id_len < MAX_IDENT) {
					id[id_len] = (char)ch;
					id_len++;
				}
				ch = get_char();
			} while (ch != EOF && isalnum(ch));
			id[id_len] = '\0';
			unget_char(ch);
			int num = sym2key(id);
			return (num == -1) ? ident : num2sym(num);
		}

		invalid();
	}
}

void
token_init()
{
	cur.row = 1;
	cur.col = 0;
	token_num = 0;
}

void
token_add(int flag)
{
	token_t *t;
	if (token_num < PREALLOC_SYM_NUM) {
		t = tokens + token_num - 1;
	} else {
		t = malloc(sizeof(token_t));
	}

	int len = strlen(id);
	t->no = token_num;
	t->value = malloc(len);
	t->type = flag;
	memcpy(t->value, id, len);

	token_tail->next = t;
	token_tail = t;
}

void
token_dump(const char *format)
{
	for (token_t *t = tokens; t != token_tail; t = t->next) {
		printf(format, t->no, t->value, symtype[t->type]);
	}
}