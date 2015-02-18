%{

/*
    basic - a small BASIC Interpreter written in C.
    Copyright (C) 2015  Thomas Cort <linuxgeek@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
%}

%union {
	int number;
	char var;
	char *string;
};

%token <number> NUMBER
%token <var> VAR
%token <string> STRING

/* keywords */
%token PRINT IF THEN GOTO INPUT LET GOSUB _RETURN CLEAR LIST RUN END

/* relational operators */
%token LTEQ LTGT LT GTEQ GTLT GT EQ

/* arithmetic operators */
%token PLUS MINUS TIMES DIVIDE

/* separators */
%token COMMA OPAREN CPAREN

%%

line	: NUMBER statement
	| statement
	;

statement	: PRINT expr_list
		| IF expression relop expression THEN statement
		| GOTO expression
		| INPUT var_list
		| LET VAR EQ expression
		| GOSUB expression
		| _RETURN
		| CLEAR
		| LIST
		| RUN
		| END
		;

expr_list	: expr_list COMMA expression
		| expr_list COMMA STRING
		| expression
		| STRING
		;

var_list	: var_list COMMA VAR
		| VAR
		;

expression	: PLUS term PLUS term
		| PLUS term MINUS term
		| MINUS term PLUS term
		| MINUS term MINUS term
		| term PLUS term
		| term MINUS term;
		| term
		;

term		: factor TIMES factor
		| factor DIVIDE factor
		| factor
		;

factor		: VAR
		| NUMBER
		| OPAREN expression CPAREN
		;

relop		: LTEQ
		| LTGT
		| LT
		| GTEQ
		| GTLT
		| GT
		| EQ
		;

%%

int yyerror(char *s) {
	fprintf(stdout, "%s\n", s);
	return -1;
}

void parseLine(char *line) {

	if (line == NULL || strlen(line) == 0) {
		return;
	}

	yy_scan_string(line);
	yyparse();
	yylex_destroy();
}

int main(int argc, char *argv[]) {

	char *line = NULL;

	if (argc == 2) {
		rl_instream = fopen(argv[1], "r");
		if (rl_instream == NULL) {
			return EXIT_FAILURE;
		}

		rl_outstream = fopen("/dev/null", "r");
		if (rl_outstream == NULL) {
			return EXIT_FAILURE;
		}
	}

	do {
		line = readline(NULL);
		if (line == NULL) {
			break;
		}
		parseLine(line);
		free(line);
	} while (1);

	if (rl_instream) {
		fclose(rl_instream);
		rl_instream = NULL;
	}

	if (rl_outstream) {
		fclose(rl_outstream);
		rl_outstream = NULL;
	}

	return EXIT_SUCCESS;
}
