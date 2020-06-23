#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "symbols.h"
#include "keywords.h"

/* extern variables of lex */
extern FILE *in, *out;
extern symbol_t *symhead, *symtail;

/* Print version and copyright */
static inline void
print_version()
{
	printf("PL/0 Analizer Copyright 2020\tShuaiCheng Zhu\n\n"
	       "This program is distributed in the hope that it will be useful,\n"
	       "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	       "GNU General Public License for more details.\n\n");
}

/* Print help and usage */
static inline void
print_help(char **argv)
{
	printf("Usage: %s infile [outfile]\n", argv[0]);
}

int
main(int argc, char *argv[])
{
	char *infile, *outfile;
	clock_t start, finish;
	double duration;
	SYMBOL flag;
	FILE *stream;

	print_version();
	if (argc < 2) {
		print_help(argv);
		return 1;
	}

	/* Open input file */
	infile = argv[1];
	if ((in = fopen(infile, "r")) == 0) {
		fprintf(stderr, "Failed on opening file \"%s\"\n", infile);
		return 1;
	}

	/* Specify output file name */
	if (argc > 2) { /* Filename was specified in args */
		outfile = argv[2];
	} else {
		/**
		 * Generate a default filename
		 * file		-> file.out
		 * file.pl0	-> file.out
		 */
		int len = strlen(infile);
		outfile = malloc(strlen(infile) + 4);
		memcpy(outfile, infile, len);

		char *t = 0;
		char *p = 0, *q = 0;

		for (t = outfile; (t = strpbrk(t + 1, ".")) != 0; p = t)
			;
		for (t = outfile; (t = strpbrk(t + 1, "/")) != 0; q = t)
			;

		if (p < q) /* Input file has no extension */
			strcat(outfile, ".out");
		else
			memcpy(p, ".out", 5);
	}

	/* Open output file */
	if ((out = fopen(outfile, "w+")) == 0) {
		fprintf(stderr, "Cannot create file \"%s\".\n", outfile);
		return 1;
	}

	/* Almost ready for lexical analysis */
	printf("Lexical Analysis:\n\n");

	/* Record start time */
	start = clock();

	/* Initialize variables */
	symbol_init();
	while (!feof(in)) {
		/* Get a symbol from input */
		flag = getsym();
		/* Abort when get invalid symbol */
		if (!flag)
			exit(1);
		/* Or add it into chain */
		symbol_add(flag);
	}

	/* Record end time */
	finish = clock();

	/* Close file streams */
	fclose(in);
	fclose(out);

	/* Print results of lex */
	printf("+-----+--------------------+--------------------+\n"
	       "|%4s |%19s |%19s |\n"
	       "+-----+--------------------+--------------------+\n",
	       "No", "Symbol", "Symbol Type");

	for (symbol_t *ptr = symhead; ptr != symtail; ptr = ptr->next) {
		symbol_dump(ptr);
	}
	printf("+-----+--------------------+--------------------+\n");

	/* Calculate time spent */
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("Lexical analysis completed (%2.3f secs)\n", duration);

	return 0;
}
