#include "parser.h"

#include <stdlib.h>

extern token_t *token_tail;
extern pos_t err;
extern const char *symtype[33];

#define invalid_token(token, assumed)                                                    \
	{                                                                                \
		fprintf(stderr,                                                          \
			"syntax:%d:%d: syntax error, expected \"%s\" but got \"%s\".\n", \
			err.row, err.col, symtype[assumed],                              \
			symtype[token->type]);                                           \
		exit(1);                                                                 \
	}

/* Get next token, abort on error */
const token_t *
next_token()
{
	/* Get a symbol from input */
	int flag = getsym();
	/* Abort if get an invalid symbol */
	if (!flag)
		exit(1);
	/* Or add it into chain */
	token_add(flag);

	return token_tail;
}

static void
assert(const token_t *token, SYMBOL sym)
{
	if (token->type != sym)
		invalid_token(token, sym)
}

void
parse()
{
	next_token();

	if (token_tail->type == constsym) { // const
		do {
			assert(next_token(), ident); // id
			assert(next_token(), eql); // =
			assert(next_token(), number); // 123
		} while (next_token()->type == comma); // ,

		assert(token_tail, semicolon); // ;
	}

	if (token_tail->type == varsym) { // var
		do {
			assert(next_token(), ident); // id
		} while (next_token()->type == comma); // ,

		assert(token_tail, semicolon); // ;
	}

	if (token_tail->type == proceduresym) { // procedure
		assert(next_token(), ident); // id
		assert(next_token(), semicolon); // ;

		assert(next_token(), beginsym); // begin
		next_token();
		do {
			parse_statement(token_tail); // a := 1
			assert(token_tail, semicolon); // ;
		} while (next_token()->type != endsym); // end
	}

	/* End of program */
	if (token_tail->type == period) // .
		return;

	parse_statement(token_tail); // a := 1
}

void
parse_statement(const token_t *token)
{
	if (token->type == ident) { // id
		assert(next_token(), becomes); // :=
		parse_expresion(next_token()); // a + 1
	}

	if (token->type == callsym) // call
		assert(next_token(), ident); // id

	if (token->type == beginsym) { // begin
		next_token();
		do {
			parse_statement(token_tail); // a := 1
			assert(next_token(), semicolon); // ;
		} while (next_token()->type != endsym); // end
	}

	if (token->type == ifsym) { // if
		parse_condition(next_token()); // a > 1

		assert(next_token(), thensym); // then

		parse_statement(next_token()); // a := 1
	}

	if (token->type == whilesym) { // while
		parse_condition(next_token()); // a > 1

		assert(token_tail, dosym); // do

		parse_statement(next_token()); // a := 1
	}

	if (token->type == readsym) { // read
		assert(next_token(), lparen); // (

		do {
			assert(next_token(), ident); // id
		} while (next_token()->type == comma); // ,

		assert(token_tail, rparen); // )
	}

	if (token->type == writesym) { // write
		assert(next_token(), lparen); // (

		next_token();
		do {
			parse_expresion(token_tail); // a + 1
		} while (next_token()->type == comma); // ,

		assert(token_tail, rparen); // )
	}
}

static void
parse_factor(const token_t *token)
{
	if (token->type == lparen) // (
		parse_expresion(next_token());

	if (token->type == ident || token->type == number) // a or 1
		return;

	invalid_token(token, ident);
}

static inline void
parse_item(const token_t *token)
{
	parse_factor(token);
	for (next_token();;) {
		if (token_tail->type == times ||
		    token_tail->type == slash) { // * or /
			parse_factor(next_token());
			continue;
		}
		break;
	}

	for (;;) {
		if (token_tail->type == plus ||
		    token_tail->type == minus) { // + and -
			parse_item(next_token());
			continue;
		}
		break;
	}
}

inline void
parse_expresion(const token_t *token)
{
	if (token->type == plus || token->type == minus) // + and -
		parse_item(next_token()); // a + 1
	else
		parse_item(token); // a + 1
}

#define skip(token, sym)                                                       \
	{                                                                      \
		if (token->type == sym)                                        \
			return;                                                \
	}

static inline void
check_compare(const token_t *token)
{
	skip(token, eql); // =
	skip(token, neq); // #
	skip(token, lss); // <
	skip(token, leq); // <=
	skip(token, gtr); // >
	skip(token, geq); // >=

	invalid_token(token, eql);
}

void
parse_condition(const token_t *token)
{
	if (token->type == oddsym) // odd
		parse_expresion(next_token()); // a + 1
	else {
		parse_expresion(token); // a + 1
		check_compare(token_tail); // >=
		parse_expresion(next_token()); // b * 2
	}
}