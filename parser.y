%{

/*
    tcb - a small BASIC Interpreter written in C.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "runtime.h"
#include "scanner.h"

#include "addop.h"
#include "command.h"
#include "expr_item.h"
#include "expr_list.h"
#include "expression.h"
#include "factor.h"
#include "line.h"
#include "mulop.h"
#include "number.h"
#include "relop.h"
#include "rnd.h"
#include "statement.h"
#include "str.h"
#include "term.h"
#include "var.h"
#include "var_list.h"

int yyerror(char *s) {
	fprintf(stdout, "%s\n", s);
	exit(EXIT_FAILURE);
	return -1;
}

%}

%union {
	int number_literal;
	char var_name;
	char *string_literal;

	struct addop *addop;
	struct command *command;
	struct expr_item *expr_item;
	struct expr_list *expr_list;
	struct expression *expression;
	struct factor *factor;
	struct mulop *mulop;
	struct number *number;
	struct relop *relop;
	struct rnd *rnd;
	struct statement *statement;
	struct str *str;
	struct term *term;
	struct var *var;
	struct var_list *var_list;
};

%token <number_literal> NUMBER
%token <var_name> VAR
%token <string_literal> STRING

/* keywords */
%token PRINT IF THEN GOTO INPUT LET GOSUB _RETURN CLEAR LIST RUN END

/* built-in functions */
%token RND

/* relational operators */
%token LTEQ LTGT LT GTEQ GTLT GT EQ

/* arithmetic operators */
%token PLUS MINUS TIMES DIVIDE

/* separators */
%token COMMA OPAREN CPAREN

/* pseudo-token */
%token EXPRESSION

%type <addop> addop
%type <command> command
%type <expr_item> expr_item
%type <expr_list> expr_list
%type <expression> expression
%type <factor> factor
%type <mulop> mulop
%type <number> number
%type <relop> relop
%type <rnd> rnd
%type <statement> statement
%type <str> str
%type <term> term
%type <var> var
%type <var_list> var_list

%%

line	: number statement { runtime_set_line(new_line($2, $1)); }
	| number { runtime_rm_line(eval_number($1)); free_number($1); }
	| statement { eval_statement($1, -1, -1); free_statement($1); }
	| command { exec_command($1); free_command($1); }
	;

command	: CLEAR { $$ = new_command(CLEAR); }
	| LIST  { $$ = new_command(LIST);  }
	| RUN   { $$ = new_command(RUN);   }
	;

statement	: PRINT expr_list                               { $$ = new_statement(  PRINT,   $2, NULL, NULL, NULL); }
		| IF expression relop expression THEN statement { $$ = new_statement(     IF,   $2,   $3,   $4,   $6); }
		| GOTO expression                               { $$ = new_statement(   GOTO,   $2, NULL, NULL, NULL); }
		| INPUT var_list                                { $$ = new_statement(  INPUT,   $2, NULL, NULL, NULL); }
		| LET var EQ expression                         { $$ = new_statement(    LET,   $2,   $4, NULL, NULL); }
		| GOSUB expression                              { $$ = new_statement(  GOSUB,   $2, NULL, NULL, NULL); }
		| _RETURN					{ $$ = new_statement(_RETURN, NULL, NULL, NULL, NULL); }
		| END                                           { $$ = new_statement(    END, NULL, NULL, NULL, NULL); }
		;

expr_list	: expr_list COMMA expr_item { $$ = new_expr_list($3,   $1); }
		| expr_item                 { $$ = new_expr_list($1, NULL); }
		;

expr_item	: expression { $$ = new_expr_item($1, NULL); }
		| str        { $$ = new_expr_item(NULL, $1); }
		;

var_list	: var_list COMMA var { $$ = new_var_list($3,   $1); }
		| var                { $$ = new_var_list($1, NULL); }
		;

addop		: PLUS  { $$ = new_addop(PLUS);  }
		| MINUS { $$ = new_addop(MINUS); }
		;

expression	: addop term addop term { $$ = new_expression(  $1, $2,   $3,   $4); }
		| addop term            { $$ = new_expression(  $1, $2, NULL, NULL); }
		| term addop term       { $$ = new_expression(NULL, $1,   $2,   $3); }
		| term                  { $$ = new_expression(NULL, $1, NULL, NULL); }
		;

mulop		: TIMES  { $$ = new_mulop(TIMES);  }
		| DIVIDE { $$ = new_mulop(DIVIDE); }
		;

term		: factor mulop factor { $$ = new_term($1,   $2,   $3); }
		| factor              { $$ = new_term($1, NULL, NULL); }
		;

factor		: var                      { $$ = new_factor(       VAR, $1); }
		| number                   { $$ = new_factor(    NUMBER, $1); }
		| OPAREN expression CPAREN { $$ = new_factor(EXPRESSION, $2); }
		| rnd                      { $$ = new_factor(       RND, $1); }
		;

rnd		: RND OPAREN expression CPAREN { $$ = new_rnd($3); }
		;

str		: STRING { $$ = new_str($1); }
		;

var		: VAR { $$ = new_var($1); }
		;

number		: NUMBER { $$ = new_number($1); }
		;

relop		: LTEQ { $$ = new_relop(LTEQ); }
		| LTGT { $$ = new_relop(LTGT); }
		| LT   { $$ = new_relop(LT);   }
		| GTEQ { $$ = new_relop(GTEQ); }
		| GTLT { $$ = new_relop(GTLT); }
		| GT   { $$ = new_relop(GT);   }
		| EQ   { $$ = new_relop(EQ);   }
		;

%%

void parseLine(char *line) {

	if (line == NULL || strlen(line) == 0) {
		return;
	}

	yy_scan_string(line);
	yyparse();
	yylex_destroy();
}
